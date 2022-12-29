#ifndef __ONIP_GRAPHICS_GL_BATCH_RENDERER_HPP__
#define __ONIP_GRAPHICS_GL_BATCH_RENDERER_HPP__

#define ONIP_MAX_UNIFORM_MODEL_MATRICES_SIZE 50
#define ONIP_MAX_UNIFORM_OVERLAY_COLOR_SIZE  50
#define ONIP_MAX_VERTEX_BUFFER_SIZE          4
#define ONIP_RAW_VERTEX_FLOAT_ELEMENT_COUNT  5
#define ONIP_RAW_VERTEX_SIZE                 sizeof(float) * ONIP_RAW_VERTEX_FLOAT_ELEMENT_COUNT

#include "onip/graphics/gl_pipeline.hpp"
#include "onip/utils/utils.hpp"
#include "onip/components/graphics_comps.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace onip {
    class GlBatchRenderer : public GlPipeline::Renderer {
    public:
        GlBatchRenderer();
        ~GlBatchRenderer() = default;

        void onDraw() override;

        void pushTransform(UUID entity_id, const Transform* transform);
        void pushVertexData(UUID entity_id, const GlPipeline::VertexData* vertices);
        void pushMaterial(UUID entity_id, const GlPipeline::Material* material, const glm::vec4* overlay_color = nullptr);
    private:
        struct Batch {
            Batch() = default;
            ~Batch() = default;

            std::vector<float> vertices {};
            std::vector<uint32_t> indices {};
            std::vector<glm::vec4> overlay_colors {};
            std::vector<glm::mat4> model_matrices {};
            const GlPipeline::Shader* shader { nullptr };
        };

        struct Reserve {
            Reserve() = default;
            ~Reserve() = default;

            UUID entity_id {};
            const GlPipeline::VertexData* vertex_data { nullptr };
            const GlPipeline::Material* material { nullptr };
            const glm::vec4* overlay_color { nullptr };
            const Transform* transform { nullptr };
        };

        void initializeVertexData();
        std::vector<GlBatchRenderer::Reserve>::iterator GlBatchRenderer::getReserve(UUID entity_id);
        void pushReserveToBatch(std::vector<Reserve>::iterator reserve);

        uint32_t m_vertex_array { 0 };
        uint32_t m_vertex_buffer { 0 };
        uint32_t m_element_buffer { 0 };
        std::vector<Batch> m_batches {};
        std::vector<Reserve> m_reserves {};
    };
}

#endif // __ONIP_GRAPHICS_GL_BATCH_RENDERER_HPP__