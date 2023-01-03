#ifndef __ONIP_GRAPHICS_GL_BATCH_RENDERER_HPP__
#define __ONIP_GRAPHICS_GL_BATCH_RENDERER_HPP__

#define ONIP_MAX_UNIFORM_MODEL_MATRICES_SIZE 50
#define ONIP_MAX_UNIFORM_OVERLAY_COLOR_SIZE  50
#define ONIP_MAX_VERTEX_BUFFER_SIZE          4
#define ONIP_RAW_VERTEX_FLOAT_ELEMENT_COUNT  9
#define ONIP_RAW_VERTEX_SIZE                 sizeof(float) * ONIP_RAW_VERTEX_FLOAT_ELEMENT_COUNT

#include "onip/graphics/gl_pipeline.hpp"
#include "onip/utils/utils.hpp"
#include "onip/components/graphics_comps.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <list>
#include <tuple>

namespace onip {
    /*
    Vertex Data Format for Phong Shaders:

        layout (location = 0) in vec3 a_position;
        layout (location = 1) in float a_overlay_color_index;
        layout (location = 2) in float a_transform_id;
        layout (location = 3) in vec2 a_uv;
        layout (location = 4) in vec2 a_diffuse_range;
        layout (location = 5) in vec2 a_specular_range;
        layout (location = 6) in vec2 a_ambient_range;

    TODO: Replace with PBR sometime in the future while also Optimizing
    */
    class GlBatchRenderer : public GlPipeline::Renderer {
    public:
        GlBatchRenderer();
        ~GlBatchRenderer() override = default;

        void onDraw() override;

        void pushTransform(UUID entity_id, const Transform* transform);
        void pushVertexData(UUID entity_id, const GlPipeline::VertexData* vertices);
        void pushMaterial(UUID entity_id, const GlPipeline::Material* material, uint32_t render_layer, const glm::vec4* overlay_color = nullptr);
        void pushOverrideTextures(UUID entity_id, const std::vector<const GlPipeline::Texture*>& override_diffuse, const std::vector<const GlPipeline::Texture*>& override_specular, const std::vector<const GlPipeline::Texture*>& override_ambient);
        void pushRenderingCameras(const std::vector<Camera*>& cameras, const std::vector<Transform*>& camera_transforms);
    private:
        struct Batch {
            Batch() = default;
            ~Batch() = default;

            std::vector<float> vertices {};
            std::vector<uint32_t> indices {};
            std::vector<uint32_t> diffuse_textures {};
            std::vector<uint32_t> specular_textures {};
            std::vector<uint32_t> ambient_textures {};
            std::vector<glm::vec4> overlay_colors {};
            std::vector<glm::mat4> model_matrices {};
            const GlPipeline::Shader* shader { nullptr };
        };

        struct RenderLayer {
            RenderLayer() = default;
            ~RenderLayer() = default;

            uint32_t layer {};
            std::list<Batch> batches {};
        };

        struct Reserve {
            Reserve() = default;
            ~Reserve() = default;

            UUID entity_id {};
            uint32_t render_layer {};
            const GlPipeline::VertexData* vertex_data { nullptr };
            const GlPipeline::Material* material { nullptr };
            const glm::vec4* overlay_color { nullptr };
            const Transform* transform { nullptr };
            std::vector<const GlPipeline::Texture*> override_diffuse_textures {};
            std::vector<const GlPipeline::Texture*> override_specular_textures {};
            std::vector<const GlPipeline::Texture*> override_ambient_textures {};
        };
        
        struct VertexBuffer {
            uint32_t id {};
            uint32_t size {};
        };

        void initializeVertexData();
        std::vector<Reserve>::iterator getReserve(UUID entity_id);
        void pushReserveIntoBatch(std::vector<Reserve>::iterator reserve);
        RenderLayer* getExistingRenderLayer(uint32_t layer);
        Batch* getExistingBatchFromLayer(RenderLayer* render_layer, const std::vector<Reserve>::iterator& reserve, float& overlay_color_index);
        Batch* addNewBatchIntoLayer(RenderLayer* render_layer, const std::vector<Reserve>::iterator& reserve, float& overlay_color_index);
        void pushReserveDataIntoBatch(const std::vector<Reserve>::iterator& reserve, Batch* batch, float overlay_color_index, float transform_index, const glm::vec2& diffuse_range, const glm::vec2& specular_range, const glm::vec2& ambient_range);
        bool pushReserveTextureDataIntoBatch(const std::vector<Reserve>::iterator& reserve, Batch* batch, glm::vec2* diffuse_range, glm::vec2* specular_range, glm::vec2* ambient_range);

        uint32_t m_vertex_array { 0 };
        VertexBuffer m_vertex_buffer {};
        VertexBuffer m_element_buffer {};
        std::list<RenderLayer> m_batches {};
        std::vector<Reserve> m_reserves {};
        std::vector<std::tuple<Transform*, Camera*>> m_rendering_cameras {};
    };
}

#endif // __ONIP_GRAPHICS_GL_BATCH_RENDERER_HPP__