#ifndef __ONIP_GRAPHICS_OPENGL_PIPELINE_HPP__
#define __ONIP_GRAPHICS_OPENGL_PIPELINE_HPP__

#include "onip/config.hpp"
#include "onip/utils/utils.hpp"
#include "onip/Core/application.hpp"
#include "onip/Core/application_layer.hpp"
#include "onip/graphics/window.hpp"

#include <array>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace onip {
    class GlPipeline : public ApplicationLayer {
    public:
        struct Shader;
        struct Texture;
        struct Material;

        class Renderer {
        public:
            virtual ~Renderer() = default;
            const std::string getName() const { return m_name; }
            virtual void onDraw() = 0;
        private:
            std::string m_name;
        };

        struct Vertex {
            glm::vec3 position;
            glm::vec3 uv;
            glm::vec3 normals;
            Texture* texture;
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

        GlPipeline();
        ~GlPipeline() override;

        ONIP_INLINE const Window* getWindow() const { return &m_window; }
        ONIP_INLINE Window* getWindow() { return &m_window; }

        ONIP_INLINE static Shader* createShader(std::string name, std::string fragment_path, std::string vertex_path) { return Application::getGraphicsPipeline()->implCreateShader(name, fragment_path, vertex_path); }
        ONIP_INLINE static Texture* createTexture(std::string name, std::string path, TextureConfig config = TextureConfig_Default) { return Application::getGraphicsPipeline()->implCreateTexture(name, path, config); }

        void onUpdate() override;
    private:
        Shader* implCreateShader(std::string name, std::string fragment_path, std::string vertex_path);
        Texture* implCreateTexture(std::string name, std::string path, TextureConfig config);

        Window m_window {};
        int m_max_texture_units {};
        std::vector<Renderer*> m_renderers {};
        std::vector<Shader*> m_shaders {};
        std::vector<Texture*> m_textures {};
        std::vector<Material*> m_materials {};
    };
}

#endif // __ONIP_GRAPHICS_OPENGL_PIPELINE_HPP__