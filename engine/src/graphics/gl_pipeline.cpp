#include "onip/graphics/gl_pipeline.hpp"
#include "onip/graphics/gl_batch_renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

namespace onip {
    struct GLPipeline::Shader {
        ~Shader() {
            glDeleteShader(id);
        }

        std::string name;
        uint32_t id;
#if ONIP_GRAPHICS_TYPES_STORE_PATH
        std::string path;
#endif // ONIP_GRAPHICS_TYPES_STORE_PATH
    };

    struct GLPipeline::Texture {
        ~Texture() {
            glDeleteTextures(1, &id);
        }

        std::string name;
        uint32_t id;
        int width;
        int height;
        int channels;
        TextureConfig config;
#if ONIP_GRAPHICS_TYPES_STORE_PATH
        std::string path;
#endif // ONIP_GRAPHICS_TYPES_STORE_PATH
    };

    struct GLPipeline::Material {
        ~Material() = default;

        std::string name;
        uint32_t shaderName;
        std::vector<uint32_t> textureIds;
#if ONIP_GRAPHICS_TYPES_STORE_PATH
        std::string path;
#endif // ONIP_GRAPHICS_TYPES_STORE_PATH
    };

    GLPipeline::GLPipeline()
        : ApplicationLayer("Graphics GLPipeline") {
        assert(glfwInit() && "failed to initialize glfw for some reason ...");

        m_window = Window(
            "Onip Game engine", Window::Resolution_Maximize,
            (Window::Config)(Window::Config_DisableVsync | Window::Config_UpdateViewportPerFrame)
        );

        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_max_texture_units);
    }

    GLPipeline::~GLPipeline() {
        for (Shader* shader : m_shaders) {
            delete shader;
        }

        for (Texture* texture : m_textures) {
            delete texture;
        }

        for (Material* material : m_materials) {
            delete material;
        }

        m_window.~Window();
        glfwTerminate();
    }

    void GLPipeline::onUpdate() {
    }
}