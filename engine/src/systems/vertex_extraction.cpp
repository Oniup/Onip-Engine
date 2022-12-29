#include "onip/systems/vertex_extraction.hpp"
#include "onip/graphics/gl_pipeline.hpp"
#include "onip/graphics/gl_pipeline.hpp"
#include "onip/core/scene_manager.hpp"
#include "onip/utils/pool.hpp"

#include <iostream>

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
            for (void* ptr : *m_target_pool) {
                if (Pool::isNull(ptr)) {
                    continue;
                }
                    
                ComponentMeta* meta = static_cast<ComponentMeta*>(ptr);
                if (Ecs::checkIfSameComponentType<Transform>(meta)) {
                    m_batch_renderer->pushTransform(meta->entity->uuid, Ecs::getComponentFromMeta<Transform>(meta));
                }
                else if (Ecs::checkIfSameComponentType<MeshRenderer>(meta)) {
                    MeshRenderer* mesh_renderer = Ecs::getComponentFromMeta<MeshRenderer>(meta);
                    m_batch_renderer->pushVertexData(meta->entity->uuid, &mesh_renderer->vertex_data);
                    m_batch_renderer->pushMaterial(meta->entity->uuid, mesh_renderer->material);
                }
                else if (Ecs::checkIfSameComponentType<SpriteRenderer>(meta)) {
                    SpriteRenderer* sprite_renderer = Ecs::getComponentFromMeta<SpriteRenderer>(meta);
                    m_batch_renderer->pushVertexData(meta->entity->uuid, &sprite_renderer->vertex_data);
                    m_batch_renderer->pushMaterial(
                        meta->entity->uuid, sprite_renderer->material, 
                        sprite_renderer->overlay_color != glm::vec4(0.0f) ? &sprite_renderer->overlay_color : nullptr
                    );
                }
            }
        }
    }
}