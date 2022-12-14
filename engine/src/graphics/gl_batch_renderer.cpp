#include "onip/graphics/gl_batch_renderer.hpp"
#include "onip/core/debug.hpp"

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#define ONIP_ELEMENT_BUFFER 0
#define ONIP_VERTEX_BUFFER  1

namespace onip {
    GlBatchRenderer::GlBatchRenderer() : GlPipeline::Renderer("Batch Renderer") {
        initializeVertexData();
    }

    void GlBatchRenderer::onDraw() {
        glBindVertexArray(m_vertex_array);
        static std::string vertex_data_string;

        for (std::tuple<Transform*, Camera*> camera : m_rendering_cameras) {
            glm::mat4 view = glm::lookAt(
                std::get<Transform*>(camera)->position, 
                glm::vec3(std::get<Transform*>(camera)->rotation) + std::get<Transform*>(camera)->position,
                std::get<Transform*>(camera)->scale
            );

            vertex_data_string = "OpenGL Batch Renderer:";
            for (RenderLayer& layer : m_batches) {
                vertex_data_string += "\nlayer [" + std::to_string(layer.layer) + "]:\n";
                for (Batch& batch : layer.batches) {
                    if (m_print_vertex_data_debug) {
                        vertex_data_string += "Vertex Data:\n";
                        size_t j = 0;
                        for (size_t i = 0; i < batch.vertices.size(); i += ONIP_RAW_VERTEX_FLOAT_ELEMENT_COUNT) {
                            if (j > 3) {
                                j = 0;
                                vertex_data_string += "\n";
                            }
                            vertex_data_string += "\t[" + std::to_string(batch.vertices[i]) + ", " + std::to_string(batch.vertices[i + 1]) + ", " + std::to_string(batch.vertices[i + 2]) + "]\t";
                            vertex_data_string += std::to_string(batch.vertices[i + 3]) + "\t" + std::to_string(batch.vertices[i + 4]) + "\n";
                            j++;
                        }
                        vertex_data_string += "\nIndex Data:\n\t";
                        for (size_t i = 0; i < batch.indices.size(); i += 3) {
                            vertex_data_string += std::to_string(batch.indices[i]) + ", " + std::to_string(batch.indices[i + 1]) + ", " + std::to_string(batch.indices[i + 2]) + "\n\t";
                        }
                    }

                    glUseProgram(batch.shader->id);

                    glUniform4fv(
                        glGetUniformLocation(batch.shader->id, "u_overlay_colors"), 
                        static_cast<int>(batch.overlay_colors.size()), &batch.overlay_colors[0][0]
                    );
                    glUniformMatrix4fv(
                        glGetUniformLocation(batch.shader->id, "u_model_matrices"),
                        static_cast<int>(batch.model_matrices.size()), false, &batch.model_matrices[0][0][0]
                    );
                    glUniformMatrix4fv(glGetUniformLocation(batch.shader->id, "u_projection_matrix"),1, false, &std::get<Camera*>(camera)->projection_matrix[0][0]);
                    glUniformMatrix4fv(glGetUniformLocation(batch.shader->id, "u_view_matrix"), 1, false, &view[0][0]);

                    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer.id);
                    if (m_vertex_buffer.size != batch.vertices.size()) {
                        m_vertex_buffer.size = static_cast<uint32_t>(batch.vertices.size());
                        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertex_buffer.size, &batch.vertices[0], GL_DYNAMIC_DRAW);
                    }
                    else {
                        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * m_vertex_buffer.size, &batch.vertices[0]);
                    }
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer.id);
                    if (m_element_buffer.size != batch.indices.size()) {
                        m_element_buffer.size = static_cast<uint32_t>(batch.indices.size());
                        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_element_buffer.size, &batch.indices[0], GL_DYNAMIC_DRAW);
                    }
                    else {
                        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * m_element_buffer.size, &batch.indices[0]);
                    }

                    glDrawElements(GL_TRIANGLES, static_cast<int>(batch.indices.size()), GL_UNSIGNED_INT, (void*)0);
                }

            }

            if (m_print_vertex_data_debug) {
                Debug::logMessage(vertex_data_string);
                m_print_vertex_data_debug = false;
            }
        }
        glBindVertexArray(0);
        glUseProgram(0);

        m_batches.clear();
        m_reserves.clear();
    }

    void GlBatchRenderer::pushTransform(UUID entity_id, const Transform* transform) {
        std::vector<Reserve>::iterator reserve = getReserve(entity_id);
        reserve->transform = transform;
        pushReserveIntoBatch(reserve);
    }

    void GlBatchRenderer::pushVertexData(UUID entity_id, const GlPipeline::VertexData* vertices) {
        std::vector<Reserve>::iterator reserve = getReserve(entity_id);
        reserve->vertex_data = vertices;
        pushReserveIntoBatch(reserve);
    }

    void GlBatchRenderer::pushMaterial(UUID entity_id, const GlPipeline::Material* material, uint32_t render_layer, const glm::vec4* overlay_color) {
        std::vector<Reserve>::iterator reserve = getReserve(entity_id);

        size_t texture_count = material->diffuse_textures.size() + material->specular_textures.size() + material->ambient_textures.size();
        size_t override_texture_count = reserve->override_diffuse_textures.size() + reserve->override_specular_textures.size() + reserve->override_ambient_textures.size();
        if (texture_count + override_texture_count < GlPipeline::getMaxTextureUnitsPerFrag()) {
            reserve->material = material;
            reserve->overlay_color = overlay_color;
            reserve->render_layer = render_layer;
            pushReserveIntoBatch(reserve);

            return;
        }
        Debug::logError(
            std::string("Cannot push Material to Reserve due to the override_texture_count (" + std::to_string(override_texture_count) + 
            ") + material texture count (" + std::to_string(texture_count) + ") exceed the max texture count (" + 
            std::to_string(GlPipeline::getMaxTextureUnitsPerFrag()) + "\n")
        );
    }

    void GlBatchRenderer::pushOverrideTextures(UUID entity_id, const std::vector<const GlPipeline::Texture*>& override_diffuse, const std::vector<const GlPipeline::Texture*>& override_specular, const std::vector<const GlPipeline::Texture*>& override_ambient) {
        std::vector<Reserve>::iterator reserve = getReserve(entity_id);

        size_t override_texture_count = override_diffuse.size() + override_specular.size() + override_ambient.size();
        size_t material_texture_count = 0;
        if (reserve->material != nullptr) {
            material_texture_count = reserve->material->diffuse_textures.size() + reserve->material->specular_textures.size() + reserve->material->ambient_textures.size();
        }
        if (override_texture_count + material_texture_count < GlPipeline::getMaxTextureUnitsPerFrag()) {
            reserve->override_diffuse_textures = override_diffuse;
            reserve->override_specular_textures = override_specular;
            reserve->override_ambient_textures = override_ambient;

            return;
        }
        Debug::logError(
            std::string("Cannot push Textures to Reserve due to the override_texture_count (" + std::to_string(override_texture_count) + 
            ") + material texture count (" + std::to_string(material_texture_count) + ") exceed the max texture count (" + 
            std::to_string(GlPipeline::getMaxTextureUnitsPerFrag()) + "\n")
        );
    }

    void GlBatchRenderer::pushRenderingCameras(const std::vector<Camera*>& cameras, const std::vector<Transform*>& camera_transforms) {
        m_rendering_cameras.clear();
        m_rendering_cameras.resize(cameras.size());
        for (size_t i = 0; i < m_rendering_cameras.size(); i++) {
            std::get<Camera*>(m_rendering_cameras[i]) = cameras[i];
            std::get<Transform*>(m_rendering_cameras[i]) = camera_transforms[i];
        }
    }

    void GlBatchRenderer::initializeVertexData() {
        if (m_vertex_array == 0) {
            glGenVertexArrays(1, &m_vertex_array);
            glBindVertexArray(m_vertex_array);

            glGenBuffers(1, &m_element_buffer.id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer.id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer.size, nullptr, GL_DYNAMIC_DRAW);

            glGenBuffers(1, &m_vertex_buffer.id);
            glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer.id);
            glBufferData(GL_ARRAY_BUFFER, m_vertex_buffer.size, nullptr, GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);   // position             = 0
            glEnableVertexAttribArray(1);   // overlay_color_index  = 3
            glEnableVertexAttribArray(2);   // transform_index      = 4
            glEnableVertexAttribArray(3);   // uv                   = 5
            glEnableVertexAttribArray(4);   // diffuse_range        = 7

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ONIP_RAW_VERTEX_SIZE, (void*)0);
            glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, ONIP_RAW_VERTEX_SIZE, (void*)(sizeof(float) * 3));
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, ONIP_RAW_VERTEX_SIZE, (void*)(sizeof(float) * 4));
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, ONIP_RAW_VERTEX_SIZE, (void*)(sizeof(float) * 5));
            glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, ONIP_RAW_VERTEX_SIZE, (void*)(sizeof(float) * 7));

            glBindVertexArray(0);
        }
    }

    std::vector<GlBatchRenderer::Reserve>::iterator GlBatchRenderer::getReserve(UUID entity_id) {
        for (std::vector<Reserve>::iterator it = m_reserves.begin(); it != m_reserves.end(); it++) {
            if (it->entity_id == entity_id) {
                return it;
            }
        }
        m_reserves.push_back(Reserve { entity_id });
        std::vector<Reserve>::iterator reserve = m_reserves.begin() + (m_reserves.size() - 1);
        reserve->entity_id = entity_id;
        return reserve;
    }

    void GlBatchRenderer::pushReserveIntoBatch(std::vector<Reserve>::iterator reserve) {
        bool add_to_batch = false;
        if (reserve->vertex_data != nullptr) {
            if (reserve->material != nullptr) {
                if (reserve->transform != nullptr) {
                    add_to_batch = true;
                }
            }
        }

        if (add_to_batch) {
            Batch* batch = nullptr;
            float overlay_color_index = 0;
            float transform_index = 0;

            // TODO: Change to use quaternions in the future
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, reserve->transform->position);
            model = glm::scale(model, reserve->transform->scale);
            model = glm::rotate(model, glm::radians(reserve->transform->rotation.w), glm::vec3(reserve->transform->rotation));

            // FIXME: Make this take a iterator not just a pointer
            RenderLayer* render_layer = getExistingRenderLayer(reserve->render_layer);
            batch = getExistingBatchFromLayer(render_layer, reserve, overlay_color_index);

            glm::vec2 diffuse_range {};
            glm::vec2 specular_range {};
            glm::vec2 ambient_range {};

            if (batch == nullptr) {
                batch = addNewBatchIntoLayer(render_layer, reserve, overlay_color_index);
                pushReserveTextureDataIntoBatch(reserve, batch, &diffuse_range, &specular_range, &ambient_range);
            }
            else {
                if (!pushReserveTextureDataIntoBatch(reserve, batch, &diffuse_range, &specular_range, &ambient_range)) {
                    // FIXME: find another batch that could possibly fit it, other add another one
                    // because after the first batch in the layer cannot fit anymore textures in, it'll
                    // always create a new batch and waste them if they can fit more textures in
                    batch = addNewBatchIntoLayer(render_layer, reserve, overlay_color_index);
                    pushReserveTextureDataIntoBatch(reserve, batch, &diffuse_range, &specular_range, &ambient_range);
                }
            }

            batch->model_matrices.emplace_back(std::move(model));
            transform_index = static_cast<float>(batch->model_matrices.size() - 1);

            pushReserveDataIntoBatch(reserve, batch, overlay_color_index, transform_index, diffuse_range, specular_range, ambient_range);

            m_reserves.erase(reserve);
        }
    }

    GlBatchRenderer::RenderLayer* GlBatchRenderer::getExistingRenderLayer(uint32_t layer) {
        // FIXME: change to use a iterator
        for (RenderLayer& render_layer : m_batches) {
            if (render_layer.layer == layer) {
                return &render_layer;
            }
        }
        return nullptr;
    }

    GlBatchRenderer::Batch* GlBatchRenderer::getExistingBatchFromLayer(RenderLayer* render_layer, const std::vector<Reserve>::iterator& reserve, float& overlay_color_index) {
        Batch* batch = nullptr;
        if (render_layer != nullptr) {
            for (Batch& existing_batch : render_layer->batches) {
                if (existing_batch.shader->id == reserve->material->shader->id) {
                    batch = &existing_batch;

                    bool found_overlay_color = false;
                    const glm::vec4* overlay_color = reserve->overlay_color != nullptr ? reserve->overlay_color : &reserve->material->color_overlay;
                    for (size_t i = 0; i < batch->overlay_colors.size(); i++) {
                        if (batch->overlay_colors[i] == *overlay_color) {
                            overlay_color_index = static_cast<float>(i);
                            found_overlay_color = true;
                            break;
                        }
                    }
                    if (!found_overlay_color) {
                        batch->overlay_colors.push_back(*overlay_color);
                        overlay_color_index = static_cast<float>(batch->overlay_colors.size() - 1);
                    }
                    break;
                }
            }
        }
        return batch;
    }

    GlBatchRenderer::Batch* GlBatchRenderer::addNewBatchIntoLayer(RenderLayer* render_layer, const std::vector<Reserve>::iterator& reserve, float& overlay_color_index) {
        if (render_layer == nullptr) {
            bool added = false;
            if (m_batches.size() > 0) {
                for (std::list<RenderLayer>::iterator layer = m_batches.begin(); layer != m_batches.end(); layer++) {
                    if (reserve->render_layer < layer->layer) {
                        if (layer == m_batches.begin()) {
                            m_batches.push_front(RenderLayer {});
                            render_layer = &m_batches.front();
                        }
                        else {
                            render_layer = &*m_batches.insert(layer--, RenderLayer {});
                        }
                        added = true;
                        break;
                    }
                }
            }
            if (!added) {
                m_batches.push_back(RenderLayer {});
                render_layer = &m_batches.front();
                render_layer->layer = reserve->render_layer;
            }
        }

        render_layer->batches.push_back(Batch {});
        Batch* batch = &render_layer->batches.back();
        batch->shader = reserve->material->shader;
        if (reserve->overlay_color != nullptr) {
            batch->overlay_colors.push_back(*reserve->overlay_color);
        }
        else {
            batch->overlay_colors.push_back(reserve->material->color_overlay);
        }
        overlay_color_index = static_cast<float>(batch->overlay_colors.size() - 1);
        return batch;
    }

    void GlBatchRenderer::pushReserveDataIntoBatch(const std::vector<Reserve>::iterator& reserve, Batch* batch, float overlay_color_index, float transform_index, const glm::vec2& diffuse_range, const glm::vec2& specular_range, const glm::vec2& ambient_range) {
        size_t vertices_start_position = batch->vertices.size();
        size_t indices_start_position = batch->indices.size();
        batch->vertices.resize(batch->vertices.size() + reserve->vertex_data->vertices.size() * ONIP_RAW_VERTEX_FLOAT_ELEMENT_COUNT);

        size_t j = 0;
        for (size_t i = vertices_start_position; i < batch->vertices.size(); i += ONIP_RAW_VERTEX_FLOAT_ELEMENT_COUNT) {
            const GlPipeline::Vertex& pipeline_vertex = reserve->vertex_data->vertices[j];

            batch->vertices[i    ] = pipeline_vertex.position.x;
            batch->vertices[i + 1] = pipeline_vertex.position.y;
            batch->vertices[i + 2] = pipeline_vertex.position.z;
            batch->vertices[i + 3] = overlay_color_index;
            batch->vertices[i + 4] = transform_index;
            batch->vertices[i + 5] = pipeline_vertex.uv.x;
            batch->vertices[i + 6] = pipeline_vertex.uv.y;
            batch->vertices[i + 7] = diffuse_range.x;
            batch->vertices[i + 8] = diffuse_range.y;

            j++;
        }

        batch->indices.insert(
            batch->indices.begin() + indices_start_position,
            reserve->vertex_data->indices.begin(), reserve->vertex_data->indices.end()
        );
        if (indices_start_position != 0) {
            int index_offset = static_cast<int>(indices_start_position) / 2 + 1;
            for (size_t i = indices_start_position; i < batch->indices.size(); i++) {
                batch->indices[i] += index_offset;
            }
        }
    }

    bool GlBatchRenderer::pushReserveTextureDataIntoBatch(const std::vector<Reserve>::iterator& reserve, Batch* batch, glm::vec2* diffuse_range, glm::vec2* specular_range, glm::vec2* ambient_range) {
        size_t current_texture_size = batch->diffuse_textures.size() + batch->specular_textures.size() + batch->ambient_textures.size();
        const std::vector<const GlPipeline::Texture*>* diffuse = reserve->override_diffuse_textures.size() == 0 ? &reserve->material->diffuse_textures : &reserve->override_diffuse_textures;
        const std::vector<const GlPipeline::Texture*>* specular = reserve->override_specular_textures.size() == 0 ? &reserve->material->specular_textures : &reserve->override_specular_textures;
        const std::vector<const GlPipeline::Texture*>* ambient = reserve->override_ambient_textures.size() == 0 ? &reserve->material->ambient_textures : &reserve->override_ambient_textures;
        size_t total_size = current_texture_size + diffuse->size() + specular->size() + ambient->size();

        if (total_size < GlPipeline::getMaxTextureUnitsPerFrag()) {
            diffuse_range->x = static_cast<float>(batch->diffuse_textures.size());
            specular_range->x = static_cast<float>(batch->specular_textures.size());
            ambient_range->x = static_cast<float>(batch->ambient_textures.size());

            batch->diffuse_textures.resize(batch->diffuse_textures.size() + diffuse->size());
            batch->specular_textures.resize(batch->specular_textures.size() + specular->size());
            batch->ambient_textures.resize(batch->ambient_textures.size() + ambient->size());

            // define the number of textures from the x (starting position) the current frag will use
            diffuse_range->y = static_cast<float>(batch->diffuse_textures.size()) - diffuse_range->x;
            specular_range->y = static_cast<float>(batch->specular_textures.size()) - specular_range->x;
            ambient_range->y = static_cast<float>(batch->ambient_textures.size()) - ambient_range->x;

            size_t j = 0;
            for (size_t i = static_cast<size_t>(diffuse_range->x); i < batch->diffuse_textures.size(); i++) {
                batch->diffuse_textures[i] = diffuse->at(j)->id;
                j++;
            }
            j = 0;
            for (size_t i = static_cast<size_t>(specular_range->x); i < batch->specular_textures.size(); i++) {
                batch->specular_textures[i] = specular->at(j)->id;
                j++;
            }
            j = 0;
            for (size_t i = static_cast<size_t>(ambient_range->x); i < batch->ambient_textures.size(); i++) {
                batch->ambient_textures[i] = ambient->at(j)->id;
                j++;
            }

            return true;
        }
        return false;
    }
}