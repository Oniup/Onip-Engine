#include "onip/graphics/gl_batch_renderer.hpp"

#include <glad/glad.h>

namespace onip {
    GLBatch::GLBatch() 
        : GLPipeline::Renderer(GLPipeline::Renderer::Type_Batch) { }

    void GLBatch::on_render() {
    }

    void GLBatch::load_data() {
        // TODO: Complete ECS
    }

    void GLBatch::push_transform_data(Transform* transform) {
        // TODO: Complete ECS
    }

    void GLBatch::push_vertices(const Vertex* vertices, size_t size) {

    }
}