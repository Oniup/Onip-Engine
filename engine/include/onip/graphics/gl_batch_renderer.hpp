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
            float transform_id {};
            float diffuse_texture_range[2] { 0.0f, 0.0f }; // [0]: start, [1]: count
            float specular_texture_range[2] { 0.0f, 0.0f };
            float ambient_texture_range[2] { 0.0f, 0.0f };
            float specular_shininess {};
        };

        struct Batch {
            Batch() = default;
            ~Batch() = default;

            std::vector<Vertex> vertex_data {};
            std::vector<uint32_t> indices_data {};
            std::vector<glm::vec4> overlay_color {};

            std::vector<GlPipeline::Texture*> diffuse_textures {};
            std::vector<GlPipeline::Texture*> specular_textures {};
            std::vector<GlPipeline::Texture*> ambient_textures {};
            std::vector<float> specular_shininess {}; // TODO: at when properly implementing specular highlights

            std::vector<const Transform*> transforms {};
            const GlPipeline::Shader* shader { nullptr };
        };

        struct BatchReserve {
            BatchReserve() = default;
            ~BatchReserve() = default;

            UUID entity_id { 0 };
            const Transform* transform { nullptr };
            const GlPipeline::Material* material { nullptr };
            const GlPipeline::Texture* override_diffuse { nullptr };
            const GlPipeline::Texture* override_specular { nullptr };
            const GlPipeline::Texture* override_ambient { nullptr };
            const std::vector<GlPipeline::Vertex>* vertices { nullptr };
            const std::vector<uint32_t>* indices { nullptr };
        };
    public:
        struct VertexRange {
            size_t vertex_start_index {};
            size_t vertex_end_index {};
            size_t indices_start_index {};
            size_t indices_end_index {};
        };

        GlBatchRenderer();
        ~GlBatchRenderer();

        void pushVertices(
            const std::vector<GlPipeline::Vertex>& vertices, const std::vector<uint32_t>& indices, 
            const GlPipeline::Material* material, const Transform* transform, const GlPipeline::Texture* override_diffuse = nullptr, 
            const GlPipeline::Texture* override_specular = nullptr, const GlPipeline::Texture* override_ambient = nullptr);
        void pushVertices(UUID entity_id, const std::vector<GlPipeline::Vertex>& vertices);
        void pushMaterial(
            UUID entity_id, const GlPipeline::Material* material, const GlPipeline::Texture* override_diffuse = nullptr, 
            const GlPipeline::Texture* override_specular = nullptr, const GlPipeline::Texture* override_ambient = nullptr);
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