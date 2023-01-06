#ifndef __ONIP_GRAPHICS_OPENGL_PIPELINE_HPP__
#define __ONIP_GRAPHICS_OPENGL_PIPELINE_HPP__

#include "onip/config.hpp"
#include "onip/utils/utils.hpp"
#include "onip/core/application.hpp"
#include "onip/core/application_layer.hpp"
#include "onip/graphics/window.hpp"

#include <array>
#include <string>
#include <string_view>
#include <vector>

#include <glm/glm.hpp>

namespace onip {
    class GlPipeline : public ApplicationLayer {
    public:
        class Renderer {
        public:
            Renderer(std::string_view name) : m_name(name) {}
            virtual ~Renderer() = default;
            const std::string getName() const { return m_name; }
            virtual void onDraw() = 0;
        private:
            std::string m_name;
        };

        struct Vertex {
            glm::vec3 position;
            glm::vec2 uv;
            glm::vec3 normals;
        };

        struct VertexData {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
        };

        enum TextureConfig {
            TextureConfig_Default                       = 0x000,
            TextureConfig_FlipVertically                = 0x001,
            TextureConfig_GenerateMipMap                = 0x002,
            TextureConfig_NearestFilteringNearest       = 0x004,
            TextureConfig_NearestFilteringFurther       = 0x008,
            TextureConfig_LinearFilteringNearest        = 0x010,
            TextureConfig_LinearFilteringFurther        = 0x020,
            TextureConfig_RepeatWrapping                = 0x040,
            TextureConfig_MirroredRepeatWrapping        = 0x080,
            TextureConfig_ClampedWrapping               = 0x100,
            TextureConfig_ClampedToBorderWrapping       = 0x200,
        };

        struct Shader {
            Shader() = default;
            ~Shader();

            std::string name {};
            uint32_t id {};

#if ONIP_GRAPHICS_TYPES_STORE_PATH
            std::string path[2] { {}, {} };
#endif // ONIP_GRAPHICS_TYPES_STORE_PATH
        };

        struct Texture {
            Texture() = default;
            ~Texture();

            std::string name {};
            uint32_t id { 0 };
            int width { 0 };
            int height { 0 };
            int channels { 0 };
            TextureConfig config { TextureConfig_Default };

#if ONIP_GRAPHICS_TYPES_STORE_PATH
            std::string path { };
#endif // ONIP_GRAPHICS_TYPES_STORE_PATH
        };

        struct Material {
            Material() = default;
            ~Material() = default;

            std::string name {};
            UUID uuid { Utils::randUint64() };
            const Shader* shader { nullptr };
            std::vector<const Texture*> diffuse_textures {};
            std::vector<const Texture*> specular_textures {};
            std::vector<const Texture*> ambient_textures {};
            float specular_shininess {};
            glm::vec4 color_overlay { glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };

#if ONIP_GRAPHICS_TYPES_STORE_PATH
            std::string path {};
#endif // ONIP_GRAPHICS_TYPES_STORE_PATH
        };

        GlPipeline();
        ~GlPipeline() override;

        ONIP_INLINE static Window* getWindow() { return &Application::getGraphicsPipeline()->m_window; }
        ONIP_INLINE static Shader* getShader(std::string_view name) { return Application::getGraphicsPipeline()->implGetShader(name); }
        ONIP_INLINE static Texture* getTexture(std::string_view name) { return Application::getGraphicsPipeline()->implGetTexture(name); }
        ONIP_INLINE static Material* getMaterial(std::string_view name) { return Application::getGraphicsPipeline()->implGetMaterial(name); }
        ONIP_INLINE static Renderer* getRenderer(std::string_view name) { return Application::getGraphicsPipeline()->implGetRenderer(name); }
        ONIP_INLINE static size_t getMaxTextureUnitsPerFrag() { return static_cast<size_t>(Application::getGraphicsPipeline()->m_max_texture_units); }

        ONIP_INLINE static Shader* createShader(std::string_view name, std::string_view fragment_path, std::string_view vertex_path) { return Application::getGraphicsPipeline()->implCreateShader(name, fragment_path, vertex_path); }
        ONIP_INLINE static Texture* createTexture(std::string_view name, std::string_view path, TextureConfig config = TextureConfig_Default) { return Application::getGraphicsPipeline()->implCreateTexture(name, path, config); }
        ONIP_INLINE static Material* createMaterial(std::string_view name, const Shader* shader) { return Application::getGraphicsPipeline()->implCreateMaterial(name, shader); }
        ONIP_INLINE static Renderer* createRenderer(Renderer*&& renderer) { return Application::getGraphicsPipeline()->implCreateRenderer(std::move(renderer)); }
        ONIP_INLINE static const char* getOpenGlVersion() { return "#version 450 core"; }

        void onUpdate() override;
    private:
        Shader* implCreateShader(std::string_view name, std::string_view fragment_path, std::string_view vertex_path);
        Texture* implCreateTexture(std::string_view name, std::string_view path, TextureConfig config);
        Material* implCreateMaterial(std::string_view name, const Shader* shader);
        Renderer* implCreateRenderer(Renderer*&& renderer);
        
        Shader* implGetShader(std::string_view name);
        Texture* implGetTexture(std::string_view name);
        Material* implGetMaterial(std::string_view name);
        Renderer* implGetRenderer(std::string_view name);

        Window m_window {};
        int m_max_texture_units {};
        std::vector<Renderer*> m_renderers {};
        std::vector<Shader*> m_shaders {};
        std::vector<Texture*> m_textures {};
        std::vector<Material*> m_materials {};
    };
}

#endif // __ONIP_GRAPHICS_OPENGL_PIPELINE_HPP__