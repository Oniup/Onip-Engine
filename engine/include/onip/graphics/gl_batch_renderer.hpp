#ifndef __ONIP_GRAPHICS_GL_BATCH_RENDERER_HPP__
#define __ONIP_GRAPHICS_GL_BATCH_RENDERER_HPP__

#include "onip/graphics/gl_pipeline.hpp"
#include "onip/utils/utils.hpp"

#include <glm/glm.hpp>

namespace onip {
    class GLBatch : public GLPipeline::Renderer {
    public:
        GLBatch();
        ~GLBatch() override = default;

        void onRender() override;
    private:
        void loadData();
        void pushTransformData(struct Transform* transform);
        void pushVertices(const Vertex* vertices, size_t size);

        std::vector<Vertex> m_vertices;
        std::vector<glm::mat4> m_transforms;
        std::vector<uint32_t> m_textures;
        bool m_update_every_buffer = true;
    };
}

#endif // __ONIP_GRAPHICS_GL_BATCH_RENDERER_HPP__