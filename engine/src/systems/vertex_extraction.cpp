#include "onip/systems/vertex_extraction.hpp"
#include "onip/graphics/gl_pipeline.hpp"
#include "onip/graphics/gl_pipeline.hpp"
#include "onip/core/scene_manager.hpp"
#include "onip/utils/pool.hpp"

namespace onip {
    GraphicsVerticesHandler::GraphicsVerticesHandler() {
        m_batch_renderer = static_cast<GlBatchRenderer*>(GlPipeline::getRenderer("Batch Renderer"));
    }

    void GraphicsVerticesHandler::onUpdate() {
        if (m_target_pool == nullptr) {
            m_target_pool = Ecs::getPoolWhichContains<Transform, SpriteRenderer, MeshRenderer>();
        }

        if (m_target_pool->getAllocations() > 0) {
            for (void* ptr : *m_target_pool) {
                if (Pool::isNull(ptr)) {
                    continue;
                }
                    
                ComponentMeta* meta = static_cast<ComponentMeta*>(ptr);
            }
        }
    }
}