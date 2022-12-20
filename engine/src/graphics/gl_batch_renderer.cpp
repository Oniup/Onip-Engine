#include "onip/graphics/gl_batch_renderer.hpp"

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

    GlBatchRenderer::VertexRange GlBatchRenderer::pushVertices(const std::vector<GlPipeline::Vertex>& vertices, const GlPipeline::Material* material, const Transform* transform) {
        Batch* batch = nullptr;
        for (size_t i = 0; i < m_batches.size(); i++) {
            if (m_batches[i]->shader == material->shader) {
                batch = m_batches[i];
                break;
            }
        }
        if (batch == nullptr) {
            m_batches.push_back(new Batch {});
            batch = m_batches.back();
            batch->shader = material->shader;
        }

        VertexRange range = { batch->vertex_data.size(), batch->vertex_data.size() + vertices.size() };
        batch->vertex_data.resize(range.end_index);

        batch->transforms.push_back(transform);
        float transform_id = (float)(batch->transforms.size() - 1);
        float material_id = -1.0f;
        for (size_t i = 0; i < batch->materials.size(); i++) {
            if (batch->materials[i]->uuid == material->uuid) {
                material_id = (float)i;
                break;
            }
        }
        if (material_id == -1.0f) {
            batch->materials.push_back(material);
            material_id = (float)(batch->materials.size() - 1);
        }

        uint32_t j = 0;
        for (size_t i = range.start_index; i < range.end_index; i += sizeof(Vertex)) {
            batch->vertex_data[i].position[0] = vertices[j].position.x;
            batch->vertex_data[i].position[1] = vertices[j].position.y;
            batch->vertex_data[i].position[2] = vertices[j].position.z;
            batch->vertex_data[i].uv[0] = vertices[j].uv.x;
            batch->vertex_data[i].uv[1] = vertices[j].uv.y;
            batch->vertex_data[i].normals[0] = vertices[j].normals.x;
            batch->vertex_data[i].normals[1] = vertices[j].normals.y;
            batch->vertex_data[i].normals[2] = vertices[j].normals.z;
            batch->vertex_data[i].material_id = material_id;
            batch->vertex_data[i].transform_id = transform_id;
            j++;
        }

        // TODO: implement pushing indices

        return range;
    }

    void GlBatchRenderer::pushVertices(UUID entity_id, const std::vector<GlPipeline::Vertex>& vertices) {
        uint32_t index = 0;
        BatchReserve* reserve = getReserve(entity_id, &index);
        reserve->vertices = &vertices;
        checkIfReserveCanPush(reserve, index);
    }

    void GlBatchRenderer::pushMaterial(UUID entity_id, const GlPipeline::Material* material) {
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

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
            glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, material_id));
            glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, transform_id));

            glBindVertexArray(0);
        }
    }

    void GlBatchRenderer::checkIfReserveCanPush(BatchReserve* reserve, uint32_t index) {
        if (reserve->transform != nullptr) {
            if (reserve->material != nullptr) {
                if (reserve->vertices != nullptr) {
                    pushVertices(*reserve->vertices, reserve->material, reserve->transform);
                    m_reserves.erase(m_reserves.begin() + index);
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