#ifndef __ONIP_GRAPHICS_GL_BATCH_RENDERER_HPP__
#define __ONIP_GRAPHICS_GL_BATCH_RENDERER_HPP__

#include "onip/graphics/gl_pipeline.hpp"
#include "onip/utils/utils.hpp"
#include "onip/components/graphics_comps.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <tuple>

namespace onip {
    class GlBatchRenderer : public GlPipeline::Renderer {
    private:
        struct Vertex {
            float position[3] { 0.0f, 0.0f, 0.0f };
            float uv[2] { 0.0f, 0.0f };
            float normals[3] { 0.0f, 0.0f, 0.0f };
            float material_id {};
            float transform_id {};
        };

        struct Batch {
            std::vector<Vertex> vertex_data {};
            std::vector<uint32_t> indices_data {};
            std::vector<const GlPipeline::Material*> materials {};
            std::vector<const Transform*> transforms {};
            const GlPipeline::Shader* shader { nullptr };
        };

        struct BatchReserve {
            UUID entity_id { 0 };
            const Transform* transform { nullptr };
            const GlPipeline::Material* material { nullptr };
            const std::vector<GlPipeline::Vertex>* vertices {};
        };
    public:
        struct VertexRange {
            size_t start_index {};
            size_t end_index {};
        };

        GlBatchRenderer();
        ~GlBatchRenderer();

        VertexRange pushVertices(const std::vector<GlPipeline::Vertex>& vertices, const GlPipeline::Material* material, const Transform* transform);
        void pushVertices(UUID entity_id, const std::vector<GlPipeline::Vertex>& vertices);
        void pushMaterial(UUID entity_id, const GlPipeline::Material* material);
        void pushTransform(UUID entity_id, const Transform* transform);

        void onDraw() override;
    private:
        void initializeVertexBuffers();
        void checkIfReserveCanPush(BatchReserve* reserve, uint32_t index);
        BatchReserve* getReserve(UUID uuid, uint32_t* index = nullptr);

        uint32_t m_buffers[3] { 0, 0, 0 };
        std::vector<Batch*> m_batches;
        std::vector<BatchReserve> m_reserves;
    };
}

#endif // __ONIP_GRAPHICS_GL_BATCH_RENDERER_HPP__