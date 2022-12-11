#include "onip/graphics/gl_batch_renderer.hpp"

#include <glad/glad.h>

namespace onip {
    GLBatch::GLBatch() 
        : GLPipeline::Renderer(GLPipeline::Renderer::Type_Batch) { }

    void GLBatch::onRender() {
    }

    void GLBatch::loadData() {
        // TODO: Complete ECS
    }

    void GLBatch::pushTransformData(Transform* transform) {
        // TODO: Complete ECS
    }

    void GLBatch::pushVertices(const Vertex* vertices, size_t size) {

    }
}