#define STB_IMAGE_IMPLEMENTATION

#include "onip/graphics/gl_pipeline.hpp"
#include "onip/graphics/gl_batch_renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

namespace onip {
    GlPipeline::GlPipeline()
        : ApplicationLayer("Graphics GLPipeline") {
        assert(glfwInit() && "failed to initialize glfw for some reason ...");

        m_window = Window(
            "Onip Game engine", Window::Resolution_Maximize,
            (Window::Config)(Window::Config_DisableVsync | Window::Config_UpdateViewportPerFrame)
        );

        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_max_texture_units);

        implCreateShader(
            "Default Sprite", "engine/assets/shaders/default_sprite.frag", "engine/assets/shaders/default_sprite.vert"
        );

        implCreateTexture(
            "Missing Texture", "engine/assets/textures/missing_texture.png", TextureConfig_Default
        );
    }

    GlPipeline::~GlPipeline() {
        for (Shader* shader : m_shaders) {
            delete shader;
        }
        for (Texture* texture : m_textures) {
            delete texture;
        }
        for (Material* material : m_materials) {
            delete material;
        }
        for (Renderer* renderer : m_renderers) {
            delete renderer;
        }
        m_window.~Window();
        glfwTerminate();
    }

    void GlPipeline::onUpdate() {
        for (Renderer* renderer : m_renderers) {
            renderer->onDraw();
        }
    }

    GlPipeline::Shader::~Shader() {
        glDeleteProgram(id);
    }

    GlPipeline::Texture::~Texture() {
        glDeleteTextures(1, &id);
    }

    GlPipeline::Shader* GlPipeline::implCreateShader(std::string name, std::string fragment_path, std::string vertex_path) {
        const char* paths[2] = { fragment_path.c_str(), vertex_path.c_str() };
        uint32_t shaders[2] = { 0, 0 };
        uint32_t shader_program = glCreateProgram();

        for (uint32_t i = 0; i < 2; i++) {
            FILE* file = fopen(paths[i], "rb");
            if (file == nullptr) {
                std::cout << "Failed to find path " << paths[i] << "\n";
                if (i != 0) {
                    glDeleteShader(shaders[0]);
                }
                return nullptr;
            }
            fseek(file, 0, SEEK_END);
            uint32_t source_length = ftell(file);
            fseek(file, 0, SEEK_SET);
            char* source = (char*)malloc(sizeof(char*) * source_length);
            fread(source, sizeof(char), source_length, file);
            source[source_length] = '\0';

            shaders[i] = glCreateShader(GL_FRAGMENT_SHADER + i);
            glShaderSource(shaders[i], sizeof(char), (const char* const*)&source, nullptr);
            glCompileShader(shaders[i]);

            int result;
            glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &result);
            if (!result) {
                glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &result);
                char* error_message = (char*)malloc(sizeof(char*) * result);
                glGetShaderInfoLog(shaders[i], result, &result, error_message);
                ONIP_ASSERT_FMT(false, "Failed to compile shader at [%s]\nOpenGL Error:\n%s", paths[i], error_message);
            }

            glAttachShader(shader_program, shaders[i]);
            fclose(file);
            free(source);
        }

        glLinkProgram(shader_program);
        int result;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
        if (!result) {
            glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &result);
            char* error_message = (char*)malloc(sizeof(char*) * result);
            glGetShaderInfoLog(shader_program, result, &result, error_message);
            ONIP_ASSERT_FMT(false, "Failed to Link Shaders to Program:\n\t- %s\n\t- %s\n", paths[0], paths[1]);
        }
        glDeleteShader(shaders[0]);
        glDeleteShader(shaders[1]);

        m_shaders.push_back(new Shader{
            name, shader_program
        });
        Shader* shader = m_shaders.back();
#ifndef NDEBUG
        shader->path[0] = fragment_path;
        shader->path[1] = vertex_path;
#endif // NDEBUG
        return shader;
    }

    GlPipeline::Texture* GlPipeline::implCreateTexture(std::string name, std::string path, TextureConfig config) {
        stbi_set_flip_vertically_on_load(config & TextureConfig_FlipVertically);

        int width, height, channels;
        uint8_t* buffer = stbi_load(path.c_str(), &width, &height, &channels, 0);
        ONIP_ASSERT_FMT(buffer, "Failed to create shader, couldn't find [%s]\n", path.c_str());

        uint32_t texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        int wrapping = GL_REPEAT;
        if (config != TextureConfig_Default) {
            if (config & TextureConfig_MirroredRepeatWrapping) {
                wrapping = GL_MIRRORED_REPEAT;
            }
            else if (config & TextureConfig_ClampedWrapping) {
                wrapping = GL_CLAMP_TO_EDGE;
            }
            else if (config & TextureConfig_ClampedToBorderWrapping) {
                wrapping = GL_CLAMP_TO_BORDER;
            }
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

        int filter_near = GL_LINEAR;
        int filter_far = GL_LINEAR_MIPMAP_LINEAR;
        if (config != TextureConfig_Default) {
            if (config & TextureConfig_NearestFilteringNearest) {
                filter_near = GL_NEAREST;
            }
            else if (config & TextureConfig_NearestFilteringFurther) {
                filter_far = config & TextureConfig_GenerateMipMap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
            }
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_near);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_far);

        int format = 0;
        switch (channels) {
        case 1:     format = GL_RED;    break;
        case 2:     format = GL_RG;     break;
        case 3:     format = GL_RGB;    break;
        case 4:     format = GL_RGBA;   break;
        }
        glTexImage2D(
            GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer
        );
        if (config == TextureConfig_Default || config & TextureConfig_GenerateMipMap) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        m_textures.push_back(new Texture {
            name, texture_id, width, height, channels, config
        });
#ifndef NDEBUG
        Texture* result = m_textures.back();
        result->path = path;
#endif // NDEBUG
        return m_textures.back();
    }

    GlPipeline::Renderer* GlPipeline::implCreateRenderer(Renderer*&& renderer) {
        m_renderers.push_back(std::move(renderer));
        return m_renderers.back();
    }

    GlPipeline::Renderer* GlPipeline::implGetRenderer(std::string_view name) {
        for (Renderer* renderer : m_renderers) {
            if (renderer->getName() == name) {
                return renderer;
            }
        }
        return nullptr;
    }

    GlPipeline::Shader* GlPipeline::implGetShader(std::string_view name) {
        for (Shader* shader : m_shaders) {
            if (shader->name == name) {
                return shader;
            }
        }
        return nullptr;
    }

    GlPipeline::Texture* GlPipeline::implGetTexture(std::string_view name) {
        for (Texture* texture : m_textures) {
            if (texture->name == name) {
                return texture;
            }
        }
        return nullptr;
    }
}