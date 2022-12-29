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
        glBindVertexArray(m_vertex_array);
        for (Batch& batch : m_batches) {
            glUseProgram(batch.shader->id);

            glUniform4fv(
                glGetUniformLocation(batch.shader->id, "u_overlay_colors"), 
                1, &batch.overlay_colors[0][0]
            );

            std::vector<float> vertices {
                 0.5f,  0.5f, 0.0f,  0, // top right
                 0.5f, -0.5f, 0.0f,  0, // bottom right
                -0.5f, -0.5f, 0.0f,  0, // bottom left
                -0.5f,  0.5f, 0.0f,  0, // top left 
            };
            std::vector<uint32_t> indices {
                0, 1, 3,   // first triangle
                1, 2, 3    // second triangle
            };

            glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), &indices[0], GL_STATIC_DRAW);

            glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, (void*)0);
        }
        glUseProgram(0);
        glBindVertexArray(0);

        // FIXME: Optimise me in the future
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

    void GlBatchRenderer::initializeVertexData() {
        if (m_vertex_array == 0) {
            glGenVertexArrays(1, &m_vertex_array);
            glBindVertexArray(m_vertex_array);

            glGenBuffers(1, &m_element_buffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 2 * ONIP_MAX_VERTEX_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);

            glGenBuffers(1, &m_vertex_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * ONIP_MAX_VERTEX_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);   // position
            glEnableVertexAttribArray(1);   // overlay_color_index
            // glEnableVertexAttribArray(2);   // transform_index;

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
            glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 3));
            // glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, transform_index));

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
            batch->vertices.resize(batch->vertices.size() + reserve->vertex_data->vertices.size());
            batch->indices.resize(batch->indices.size() + reserve->vertex_data->indices.size());

            size_t j = 0;
            for (size_t i = vertices_start_position; i < batch->vertices.size(); i++) {
                Vertex& vertex = batch->vertices[i];
                const GlPipeline::Vertex& pipeline_vertex = reserve->vertex_data->vertices[j];

                vertex.position[0] = pipeline_vertex.position.x;
                vertex.position[1] = pipeline_vertex.position.y;
                vertex.position[2] = pipeline_vertex.position.z;
                vertex.overlay_color_index = overlay_color_index;
                vertex.transform_index = transform_index;
                j++;
            }

            batch->indices.insert(
                batch->indices.begin() + indices_start_position,
                reserve->vertex_data->indices.begin(), reserve->vertex_data->indices.end()
            );

            m_reserves.erase(reserve);
        }
    }
}