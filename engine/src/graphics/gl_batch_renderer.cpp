#include "onip/graphics/gl_batch_renderer.hpp"

#include <iostream>
#include <glad/glad.h>

namespace onip {
    GlBatchRenderer::GlBatchRenderer() : GlPipeline::Renderer("Batch Renderer") {
        initializeVertexBuffers();
    }

    GlBatchRenderer::~GlBatchRenderer() {
        for (Batch* batch : m_batches) {
            delete batch;
        }
    }

    void GlBatchRenderer::pushVertices(
            const std::vector<GlPipeline::Vertex>& vertices, const std::vector<uint32_t>& indices, 
            const GlPipeline::Material* material, const Transform* transform, const GlPipeline::Texture* override_diffuse, 
            const GlPipeline::Texture* override_specular, const GlPipeline::Texture* override_ambient)
    {
        int number_of_textures = material->diffuse_textures.size() + material->specular_textures.size() + material->ambient_textures.size();
        if (number_of_textures > GlPipeline::getMaxTextureUnitsPerFrag()) {
            std::cout << "Cannot render this as there are too many textures being rendered in one frag, max is " << GlPipeline::getMaxTextureUnitsPerFrag() + "\n";
        }

        // Getting the current Batch and pushing the material unless it already exists
        Batch* batch = nullptr;
        glm::vec2 diffuse_texture_range = glm::vec2(-1.0f, -1.0f);
        glm::vec2 specular_texture_range = glm::vec2(-1.0f, -1.0f);
        glm::vec2 ambient_texture_range = glm::vec2(-1.0f, -1.0f);

        float material_id = -1.0f;
        for (size_t i = 0; i < m_batches.size(); i++) {
            if (m_batches[i]->shader == material->shader) {
                size_t current_texture_size = m_batches[i]->diffuse_textures.size() + m_batches[i]->specular_textures.size() +
                    m_batches[i]->ambient_textures.size();
                if (current_texture_size < GlPipeline::getMaxTextureUnitsPerFrag()) {
                    batch = m_batches[i];

                    // Texture handling
                    std::vector<GlPipeline::Texture*> new_diffuse_textures;
                    std::vector<GlPipeline::Texture*> new_specular_textures;
                    std::vector<GlPipeline::Texture*> new_ambient_textures;
                    for (size_t i = 0; i < 3; i++) {
                        const std::vector<GlPipeline::Texture*>* material_textures = nullptr;
                        std::vector<GlPipeline::Texture*>* batch_textures = nullptr;
                        std::vector<GlPipeline::Texture*>* new_textures = nullptr;
                        switch (i) {
                        case 0:
                            material_textures = &material->diffuse_textures;
                            batch_textures = &batch->diffuse_textures;
                            new_textures = &new_diffuse_textures;
                        break;
                        case 1:
                            material_textures = &material->specular_textures;
                            batch_textures = &batch->specular_textures;
                            new_textures = &new_specular_textures;
                        break;
                        case 2:
                            material_textures = &material->ambient_textures;
                            batch_textures = &batch->ambient_textures;
                            new_textures = &new_ambient_textures;
                        break;
                        }
                    }

                    size_t new_texture_size = new_diffuse_textures.size() + new_specular_textures.size() + new_ambient_textures.size() +  
                        batch->diffuse_textures.size() + batch->specular_textures.size() + batch->ambient_textures.size();
                    if (new_texture_size > GlPipeline::getMaxTextureUnitsPerFrag()) {
                        batch = nullptr;
                        continue;
                    }

                    break;
                }
            }
        }
        // Couldn't find a batch with the same shader in use, so create a new one and push material
        if (batch == nullptr) {
            m_batches.push_back(new Batch {});
            batch = m_batches.back();
            batch->shader = material->shader;
            batch->diffuse_textures = material->diffuse_textures;
            batch->specular_textures = material->specular_textures;
            batch->ambient_textures = material->ambient_textures;
            batch->specular_shininess.push_back(material->specular_shininess);
        }

        // Push transform component to batches transform buffer
        batch->transforms.push_back(transform);
        float transform_id = (float)(batch->transforms.size() - 1);
        
        // Finding the vertex and index data ranges being used
        VertexRange range = { 
            batch->vertex_data.size(), batch->vertex_data.size() + vertices.size(), 
            batch->indices_data.size(), batch->indices_data.size() + (batch->vertex_data.size() / 3)
        };

        // Pushing the vertex and index data to the buffer
        batch->vertex_data.resize(range.vertex_end_index);
        batch->indices_data.resize(range.indices_end_index);
        uint32_t j = 0;
        for (size_t i = range.vertex_start_index; i < range.vertex_end_index; i++) {
            batch->vertex_data[i].position[0] = vertices[j].position.x;
            batch->vertex_data[i].position[1] = vertices[j].position.y;
            batch->vertex_data[i].position[2] = vertices[j].position.z;
            batch->vertex_data[i].uv[0] = vertices[j].uv.x;
            batch->vertex_data[i].uv[1] = vertices[j].uv.y;
            batch->vertex_data[i].normals[0] = vertices[j].normals.x;
            batch->vertex_data[i].normals[1] = vertices[j].normals.y;
            batch->vertex_data[i].normals[2] = vertices[j].normals.z;
            batch->vertex_data[i].transform_id = transform_id;
            batch->vertex_data[i].diffuse_texture_range[0] = diffuse_texture_range[0];
            batch->vertex_data[i].diffuse_texture_range[1] = diffuse_texture_range[1];
            batch->vertex_data[i].specular_texture_range[0] = specular_texture_range[0];
            batch->vertex_data[i].specular_texture_range[1] = specular_texture_range[1];
            batch->vertex_data[i].ambient_texture_range[0] = ambient_texture_range[0];
            batch->vertex_data[i].ambient_texture_range[1] = ambient_texture_range[1];
            batch->vertex_data[i].specular_shininess = material->specular_shininess;
            j++;
        }
        batch->indices_data.insert(batch->indices_data.begin() + range.indices_start_index, indices.begin(), indices.end());
    }

    void GlBatchRenderer::pushVertices(UUID entity_id, const std::vector<GlPipeline::Vertex>& vertices) {
        uint32_t index = 0;
        BatchReserve* reserve = getReserve(entity_id, &index);
        reserve->vertices = &vertices;
        checkIfReserveCanPush(reserve, index);
    }

    void GlBatchRenderer::pushMaterial(
            UUID entity_id, const GlPipeline::Material* material, const GlPipeline::Texture* override_diffuse, 
            const GlPipeline::Texture* override_specular, const GlPipeline::Texture* override_ambient)
    {
        uint32_t index = 0;
        BatchReserve* reserve = getReserve(entity_id, &index);
        reserve->material = material;
        checkIfReserveCanPush(reserve, index);
    }

    void GlBatchRenderer::pushTransform(UUID entity_id, const Transform* transform) {
        uint32_t index = 0;
        BatchReserve* reserve = getReserve(entity_id, &index);
        reserve->transform = transform;
        checkIfReserveCanPush(reserve, index);
    }

    void GlBatchRenderer::onDraw() {
        for (Batch* batch : m_batches) {
            delete batch;
        }
        m_batches.clear();
        m_reserves.clear();
    }

    void GlBatchRenderer::initializeVertexBuffers() {
        if (m_buffers[0] == 0) {
            glGenVertexArrays(1, &m_buffers[0]);
            glGenBuffers(2, m_buffers + sizeof(uint32_t));

            glBindVertexArray(m_buffers[0]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[1]);
            glBindBuffer(GL_ARRAY_BUFFER, m_buffers[2]);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);
            glEnableVertexAttribArray(5);
            glEnableVertexAttribArray(6);
            glEnableVertexAttribArray(7);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
            glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, transform_id));
            glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, diffuse_texture_range));
            glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, specular_texture_range));
            glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, ambient_texture_range));
            glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, specular_shininess));

            glBindVertexArray(0);
        }
    }

    void GlBatchRenderer::checkIfReserveCanPush(BatchReserve* reserve, uint32_t index) {
        if (reserve->transform != nullptr) {
            if (reserve->material != nullptr) {
                if (reserve->vertices != nullptr) {
                    if (reserve->indices != nullptr ) {
                        pushVertices(
                            *reserve->vertices, *reserve->indices, reserve->material, reserve->transform, 
                            reserve->override_diffuse, reserve->override_specular, reserve->override_ambient
                        );
                        m_reserves.erase(m_reserves.begin() + index);
                    }
                }
            }
        }
    }

    GlBatchRenderer::BatchReserve* GlBatchRenderer::getReserve(UUID uuid, uint32_t* index) {
        uint32_t i = 0;
        for (; i < m_reserves.size(); i++) {
            if (m_reserves[i].entity_id == uuid) {
                if (index != nullptr) {
                    *index = i;
                }
                return &m_reserves[i];
            }
        }

        m_reserves.push_back({});
        return &m_reserves.back();
    }
}