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
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        uint32_t textureIndex;
    };

    class GLPipeline : public ApplicationLayer {
    public:
        struct Shader;
        struct Texture;
        struct Material;

        enum TextureConfig {
            TextureConfig_Default                       = -1,
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

        class Renderer {
        public:
            enum Type {
                Type_Batch = 0,
                Type_Instance,
            };

            Renderer(Type type)
                : m_type(type) {}

            const Type getType() const { return m_type; }

            virtual ~Renderer() = default;
            virtual void onRender() = 0;
        private:
            Type m_type;
        };

        GLPipeline();
        ~GLPipeline() override;

        ONIP_INLINE const Window* getWindow() const { return &m_window; }
        ONIP_INLINE Window* getWindow() { return &m_window; }

        void onUpdate() override;
    private:
        Window m_window;
        int m_max_texture_units;
        std::vector<Renderer*> m_renderers;

        std::vector<Shader*> m_shaders;
        std::vector<Texture*> m_textures;
        std::vector<Material*> m_materials;
    };
}

#endif // __ONIP_GRAPHICS_OPENGL_PIPELINE_HPP__