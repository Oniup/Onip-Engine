#include "onip/systems/vertex_extraction.hpp"
#include "onip/graphics/gl_pipeline.hpp"
#include "onip/graphics/gl_pipeline.hpp"
#include "onip/core/scene_manager.hpp"
#include "onip/utils/pool.hpp"

namespace onip {
    GraphicsVertexExtractionSystem::GraphicsVertexExtractionSystem() {
        m_batch_renderer = static_cast<GlBatchRenderer*>(GlPipeline::getRenderer("Batch Renderer"));
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
                if (Transform::getId() == meta->comp_id) {
                    m_batch_renderer->pushTransform(meta->entity->uuid, Ecs::getComponentFromMeta<Transform>(meta));
                }
                else if (MeshRenderer::getId() == meta->comp_id) {
                    MeshRenderer* mesh_renderer = Ecs::getComponentFromMeta<MeshRenderer>(meta);
                    m_batch_renderer->pushVertices(meta->entity->uuid, mesh_renderer->vertices);
                    m_batch_renderer->pushMaterial(meta->entity->uuid, mesh_renderer->material);
                }
                else if (SpriteRenderer::getId() == meta->comp_id) {
                    SpriteRenderer* sprite_renderer = Ecs::getComponentFromMeta<SpriteRenderer>(meta);
                    m_batch_renderer->pushVertices(meta->entity->uuid, sprite_renderer->vertices);
                    m_batch_renderer->pushMaterial(meta->entity->uuid, sprite_renderer->material);
                }
            }
        }
    }
}