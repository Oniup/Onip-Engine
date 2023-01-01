#include "onip/systems/vertex_extraction.hpp"
#include "onip/graphics/gl_pipeline.hpp"
#include "onip/graphics/gl_pipeline.hpp"
#include "onip/core/scene_manager.hpp"
#include "onip/utils/pool.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace onip {
    GraphicsVertexExtractionSystem::GraphicsVertexExtractionSystem(GlBatchRenderer* renderer) {
        if (renderer != nullptr) {
            m_batch_renderer = renderer;
        }
        else {
            m_batch_renderer = static_cast<GlBatchRenderer*>(GlPipeline::getRenderer("Batch Renderer"));
        }
        // TODO: add instance rendering here ...
    }

    void GraphicsVertexExtractionSystem::onUpdate() {
        if (m_target_pool == nullptr) {
            m_target_pool = Ecs::getPoolWhichContains<Transform, SpriteRenderer, MeshRenderer>();
        }

        if (m_target_pool->getAllocations() > 0) {
            std::vector<Camera*> camera_cameras = std::move(Ecs::getAllOfComponent<Camera>());
            std::vector<Transform*> camera_transforms {};
            camera_transforms.resize(camera_cameras.size());

            for (void* ptr : *m_target_pool) {
                if (Pool::isNull(ptr)) {
                    continue;
                }
                    
                ComponentMeta* meta = static_cast<ComponentMeta*>(ptr);
                if (Ecs::checkIfSameComponentType<Transform>(meta)) {
                    Transform* transform = Ecs::getComponentFromMeta<Transform>(meta);
                    bool camera_found = false;
                    for (size_t i = 0; i < camera_cameras.size(); i++) {
                        Camera* camera = camera_cameras[i];
                        if (Ecs::getMetaFromComponent(camera)->entity->uuid == meta->entity->uuid) {
                            camera_found = true;
                            camera_transforms[i] = transform;
                            break;
                        }
                    }
                    if (camera_found) {
                        continue;
                    }
                    m_batch_renderer->pushTransform(meta->entity->uuid, transform);
                }
                else if (Ecs::checkIfSameComponentType<MeshRenderer>(meta)) {
                    MeshRenderer* mesh_renderer = Ecs::getComponentFromMeta<MeshRenderer>(meta);
                    m_batch_renderer->pushVertexData(meta->entity->uuid, &mesh_renderer->vertex_data, mesh_renderer->render_layer);
                    m_batch_renderer->pushMaterial(meta->entity->uuid, mesh_renderer->material, mesh_renderer->render_layer);
                }
                else if (Ecs::checkIfSameComponentType<SpriteRenderer>(meta)) {
                    SpriteRenderer* sprite_renderer = Ecs::getComponentFromMeta<SpriteRenderer>(meta);
                    m_batch_renderer->pushVertexData(meta->entity->uuid, &sprite_renderer->vertex_data, sprite_renderer->render_layer);
                    m_batch_renderer->pushMaterial(
                        meta->entity->uuid, sprite_renderer->material, sprite_renderer->render_layer,
                        sprite_renderer->overlay_color != glm::vec4(0.0f) ? &sprite_renderer->overlay_color : nullptr
                    );
                }
            }

            if (GlPipeline::getWindow()->sizeChanged()) {
                for (Camera* camera : camera_cameras) {
                    switch (camera->projection) {
                    case Camera::Projection_Orthographic: {
                        float x = static_cast<float>(GlPipeline::getWindow()->getSize().x) / camera->ortho_scale;
                        float y = static_cast<float>(GlPipeline::getWindow()->getSize().y) / camera->ortho_scale;
                        camera->projection_matrix = glm::ortho(-x, x, -y, y);
                    } break;
                    case Camera::Projection_Perspective: {
                        camera->projection_matrix = glm::perspective(
                            glm::radians(camera->fov), 
                            static_cast<float>(GlPipeline::getWindow()->getSize().x / GlPipeline::getWindow()->getSize().y),
                            camera->near_plane, camera->far_plane
                        );
                    } break;
                    }
                }
            }

            m_batch_renderer->pushRenderingCameras(camera_cameras, camera_transforms);
        }
    }
}