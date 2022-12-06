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

        m_renderers.push_back(new GLBatch());
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

    void GLPipeline::on_update() {
    }

//     GLPipeline::Shader* GLPipeline::impl_create_shader(const std::string& name, const std::string& fragment_path, const std::string& vertex_path) {
//         onipAssert(name.c_str(), "Need to provide a name for the creating a Shader");
//         onipAssert(fragment_path.c_str(), "Need to provide a Fragment Shader Source Path for creating a Shader");
//         onipAssert(vertex_path.c_str(), "Need to provide a Vertex Shader Source Path for creating a Shader");

//         const char* paths[] = { fragment_path.c_str(), vertex_path.c_str() };
//         uint32_t shaders[] = { 0, 0 };

//         for (int i = 0; i < 2; i++) {
//             FILE* file = fopen(paths[i], "rb");
//             onipAssert(file, "Failed to find the %s Shader file from path: %s", i == 0 ? "Fragment" : "Vertex", paths[i]);

//             fseek(file, 0, SEEK_END);
//             int fileLength = ftell(file);
//             fseek(file, 0, SEEK_SET);

//             char* source_buffer = static_cast<char*>(malloc(sizeof(char*) * fileLength));
//             onipAssert(source_buffer, "Failed to allocate memory to source for reading the %s Shader file from path: %s", i == 0 ? "Fragment" : "Vertex", paths[i])
//             fread(source_buffer, fileLength, sizeof(char), file);
//             source_buffer[fileLength] = '\0';
//             fclose(file);

//             shaders[i] = glCreateShader(GL_FRAGMENT_SHADER + i);
//             glShaderSource(shaders[i], 1, const_cast<const char* const*>(&source_buffer), &fileLength);
//             glCompileShader(shaders[i]);
//             free(source_buffer);
            
//             int result;
//             glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &result);
//             if (!result) {
//                 glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &result);
//                 char* error_message = static_cast<char*>(malloc(sizeof(char*) * result));
//                 glGetShaderInfoLog(shaders[i], result, &result, error_message);
//                 onipAssert(
//                     result, "Failed to compile %s Shader from the path %s\nOpenGL Error:\n%s", 
//                     i == 0 ? "Fragment" : "Vertex", paths[i], error_message
//                 );
//             }
//         }

//         m_shaders.push_back(new Shader);
//         Shader* instance = m_shaders.last();
//         instance->name = name;
// #if ONIP_GRAPHICS_TYPES_STORE_PATH
//         instance->path = std::string("Fragment Path: ") + fragment_path + "\nVertex Path: " + vertex_path;
// #endif // ONIP_GRAPHICS_TYPES_STORE_PATH

//         instance->id = glCreateProgram();
//         glAttachShader(instance->id, shaders[0]); // Fragment
//         glAttachShader(instance->id, shaders[1]); // Vertex
//         glLinkProgram(instance->id);

//         int result;
//         glGetProgramiv(instance->id, GL_LINK_STATUS, &result);
//         if (!result) {
//             glGetProgramiv(instance->id, GL_INFO_LOG_LENGTH, &result);
//             char* error_message = static_cast<char*>(malloc(sizeof(char*) * result));
//             glGetProgramInfoLog(instance->id, result, &result, error_message);
//             onipAssert(result, "Failed to link the following Shaders:\n%s", instance->path.c_str());
//         }

//         return instance;
//     }

//     GLPipeline::Texture* GLPipeline::impl_create_texture(const std::string& name, const std::string& path, TextureConfig config) {
//         onipAssert(name.c_str(), "Need to provide a name for creating a Texture");
//         onipAssert(path.c_str(), "Need to provide a path for creating a Texture");

//         if (config == TextureConfig_Default) {
//             config = static_cast<TextureConfig>(
//                 TextureConfig_RepeatWrapping | TextureConfig_LinearFilteringFurther |
//                 TextureConfig_LinearFilteringNearest | TextureConfig_GenerateMipMap |
//                 TextureConfig_FlipVertically
//             );
//         }

//         int wrapping = GL_REPEAT;
//         if (config & TextureConfig_MirroredRepeatWrapping) {
//             wrapping = GL_MIRRORED_REPEAT;
//         }
//         else if (config & TextureConfig_ClampedWrapping) {
//             wrapping = GL_CLAMP_TO_EDGE;
//         }
//         else if (config & TextureConfig_ClampedToBorderWrapping) {
//             wrapping = GL_CLAMP_TO_BORDER;
//         }

//         int filtering_nearest = config & TextureConfig_LinearFilteringNearest ? GL_LINEAR : GL_NEAREST;
//         int filtering_further = filtering_nearest;
//         if (config & TextureConfig_GenerateMipMap) {
//             filtering_further = config & TextureConfig_LinearFilteringFurther ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST;
//         }


//         m_textures.push(new Texture);
//         Texture* texture = m_textures.last();
//         stbi_set_flip_vertically_on_load(config & TextureConfig_FlipVertically ? true : false);
//         uint8_t* buffer = stbi_load(path.c_str(), &texture->width, &texture->height, &texture->channels, 0);
//         onipAssert(buffer, "Failed to create Texture due to stbi_load returned a buffer that is Null");
//         texture->config = config;
//         texture->name = name;

//         glGenTextures(1, &texture->id);
//         glBindTexture(GL_TEXTURE_2D, texture->id);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering_nearest);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering_further);

//         int color_format;
//         switch (texture->channels) {
//         case 0:
//             onipAssert(
//                 texture->channels != 0, 
//                 "There needs to be at least 1 Channel within the Image Source to create a Texture, Path: %s",
//                 path.c_str()
//             );
//             break;
//         case 1: color_format = GL_RED;   break;
//         case 2: color_format = GL_RG;    break;
//         case 3: color_format = GL_RGB;   break;
//         case 4: color_format = GL_RGBA;  break;
//         }

//         glTexImage2D(
//             GL_TEXTURE_2D, 0, color_format, texture->width, texture->height, 0,
//             color_format, GL_UNSIGNED_BYTE, buffer
//         );
//         if (config & TextureConfig_GenerateMipMap) {
//             glGenerateMipmap(GL_TEXTURE_2D);
//         }

//         glBindTexture(GL_TEXTURE_2D, 0);
//         stbi_image_free(buffer);

//         return texture;
//     }

//     GLPipeline::Material* GLPipeline::impl_create_material(const std::string& name, Shader* shader, Texture* textures, size_t textureSize) {
//         onipAssert(name.c_str(), "Must give the Material a Name to create one --> it was set to nullptrCreate");
//         onipAssert(shader, "Failed to find Shader --> it doesn't exist");
//         return nullptr;
//     }

        // TODO: complete a basic renderer first before implementing this
    // }
}