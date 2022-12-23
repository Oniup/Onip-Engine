#ifndef __ONIP_COMPONENTS_GRAPHICS_COMPS_HPP__
#define __ONIP_COMPONENTS_GRAPHICS_COMPS_HPP__

#include "onip/graphics/gl_pipeline.hpp"

#include <glm/glm.hpp>
#include <vector>


namespace onip {
    struct Transform {
        Transform() = default;
        ~Transform() = default;
        static constexpr uint32_t getId() { return 0; }

        glm::vec3 position { glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 scale { glm::vec3(1.0f, 1.0f, 1.0f) };
        glm::vec4 rotation { glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) };
    };

    struct SpriteRenderer {
        SpriteRenderer() = default;
        ~SpriteRenderer() = default;
        static constexpr uint32_t getId() { return 1; }

        bool flip_x { false };
        bool flip_y { false };
        std::vector<GlPipeline::Vertex> vertices { {}, {}, {}, {} };
        glm::vec4 overlay_color { glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
        GlPipeline::Texture* sprite { nullptr };
        GlPipeline::Texture* sprite_specular { nullptr };
        GlPipeline::Texture* sprite_ambient { nullptr };
        GlPipeline::Material* material { nullptr };
    };

    struct MeshRenderer {
        MeshRenderer() = default;
        ~MeshRenderer() = default;
        static constexpr uint32_t getId() { return 2; }

        std::vector<GlPipeline::Vertex> vertices {};
        GlPipeline::Material* material { nullptr };
    };

    struct Camera {
        Camera() = default;
        ~Camera() = default;
        static constexpr uint32_t getId() { return 3; }

        glm::vec3 up { glm::vec3(0.0f, 1.0f, 0.0f) };
        glm::vec3 forward { glm::vec3(0.0f, 0.0f, 1.0f) };
    };
}

#endif // __ONIP_COMPONENTS_GRAPHICS_COMPS_HPP__