#include "onip/graphics/gl_batch_renderer.hpp"

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
        // FIXME: Optimise and make more robust in the future
        glBindVertexArray(m_vertex_array);
        for (std::tuple<Transform*, Camera*> camera : m_rendering_cameras) {
            glm::mat4 view = glm::lookAt(
                std::get<Transform*>(camera)->position, 
                glm::vec3(std::get<Transform*>(camera)->rotation) + std::get<Transform*>(camera)->position,
                std::get<Transform*>(camera)->scale
            );

            for (Batch& batch : m_batches) {
#ifndef NDEBUG
                static bool first_time_printing_for_first_batch = true;
                if (first_time_printing_for_first_batch) {
                    std::cout << "vertex data:\n";
                    size_t j = 0;
                    for (size_t i = 0; i < batch.vertices.size(); i += ONIP_RAW_VERTEX_FLOAT_ELEMENT_COUNT) {
                        if (j > 3) {
                            j = 0;
                            std::cout << "\n";
                        }
                        std::cout << "[" << batch.vertices[i] << ", " << batch.vertices[i + 1] << ", " << batch.vertices[i + 2] << "]\t";
                        std::cout << batch.vertices[i + 3] << "\t" << batch.vertices[i + 4] << "\n";
                        j++;
                    }
                    std::cout << "\nindex data:\n";
                    for (size_t i = 0; i < batch.indices.size(); i += 3) {
                        std::cout << batch.indices[i] << ", " << batch.indices[i + 1] << ", " << batch.indices[i + 2] << "\n";
                    }
                    first_time_printing_for_first_batch = false;
                }
#endif // NDEBUG

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
                    m_vertex_buffer.size = batch.vertices.size();
                    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertex_buffer.size, &batch.vertices[0], GL_DYNAMIC_DRAW);
                }
                else {
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * m_vertex_buffer.size, &batch.vertices[0]);
                }
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer.id);
                if (m_element_buffer.size != batch.indices.size()) {
                    m_element_buffer.size = batch.indices.size();
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_element_buffer.size, &batch.indices[0], GL_DYNAMIC_DRAW);
                }
                else {
                    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * m_element_buffer.size, &batch.indices[0]);
                }

                glDrawElements(GL_TRIANGLES, static_cast<int>(batch.indices.size()), GL_UNSIGNED_INT, (void*)0);
            }

            glUseProgram(0);
            glBindVertexArray(0);
        }

        m_batches.clear();
        m_reserves.clear();
    }

    void GlBatchRenderer::pushTransform(UUID entity_id, const Transform* transform) {
        std::vector<Reserve>::iterator reserve = getReserve(entity_id);
        reserve->transform = transform;
        pushReserveToBatch(reserve);
    }

    void GlBatchRenderer::pushVertexData(UUID entity_id, const GlPipeline::VertexData* vertices) {
        std::vector<Reserve>::iterator reserve = getReserve(entity_id);
        reserve->vertex_data = vertices;
        pushReserveToBatch(reserve);
    }

    void GlBatchRenderer::pushMaterial(UUID entity_id, const GlPipeline::Material* material, const glm::vec4* overlay_color) {
        std::vector<Reserve>::iterator reserve = getReserve(entity_id);
        reserve->material = material;
        reserve->overlay_color = overlay_color;
        pushReserveToBatch(reserve);
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

            glEnableVertexAttribArray(0);   // position
            glEnableVertexAttribArray(1);   // overlay_color_index
            glEnableVertexAttribArray(2);   // transform_index;

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ONIP_RAW_VERTEX_SIZE, (void*)0);
            glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, ONIP_RAW_VERTEX_SIZE, (void*)(sizeof(float) * 3));
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, ONIP_RAW_VERTEX_SIZE, (void*)(sizeof(float) * 4));

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

    void GlBatchRenderer::pushReserveToBatch(std::vector<Reserve>::iterator reserve) {
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

            for (Batch& existing_batch : m_batches) {
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

            if (batch == nullptr) {
                m_batches.push_back({});
                batch = &m_batches.back();
                batch->shader = reserve->material->shader;
                if (reserve->overlay_color != nullptr) {
                    batch->overlay_colors.push_back(*reserve->overlay_color);
                }
                else {
                    batch->overlay_colors.push_back(reserve->material->color_overlay);
                }
                overlay_color_index = static_cast<float>(batch->overlay_colors.size() - 1);
            }

            batch->model_matrices.emplace_back(std::move(model));
            transform_index = static_cast<float>(batch->model_matrices.size() - 1);

            size_t vertices_start_position = batch->vertices.size();
            size_t indices_start_position = batch->indices.size();
            batch->vertices.resize(batch->vertices.size() + reserve->vertex_data->vertices.size() * ONIP_RAW_VERTEX_FLOAT_ELEMENT_COUNT);
            // batch->indices.resize(batch->indices.size() + reserve->vertex_data->indices.size());

            size_t j = 0;
            for (size_t i = vertices_start_position; i < batch->vertices.size(); i += ONIP_RAW_VERTEX_FLOAT_ELEMENT_COUNT) {
                const GlPipeline::Vertex& pipeline_vertex = reserve->vertex_data->vertices[j];

                batch->vertices[i    ] = pipeline_vertex.position.x;
                batch->vertices[i + 1] = pipeline_vertex.position.y;
                batch->vertices[i + 2] = pipeline_vertex.position.z;
                batch->vertices[i + 3] = overlay_color_index;
                batch->vertices[i + 4] = transform_index;

                j++;
            }

            batch->indices.insert(
                batch->indices.begin() + indices_start_position,
                reserve->vertex_data->indices.begin(), reserve->vertex_data->indices.end()
            );
            if (indices_start_position != 0) {
                int index_offset = indices_start_position / 2 + 1;
                for (size_t i = indices_start_position; i < batch->indices.size(); i++) {
                    batch->indices[i] += index_offset;
                }
            }

            m_reserves.erase(reserve);
        }
    }
}