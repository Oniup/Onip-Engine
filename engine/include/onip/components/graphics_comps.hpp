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
        glm::vec4 overlay_color { glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };

        GlPipeline::Material* material { nullptr };
        GlPipeline::Texture* sprite { nullptr };
        GlPipeline::Texture* sprite_specular { nullptr };
        GlPipeline::Texture* sprite_ambient { nullptr };

        GlPipeline::VertexData vertex_data { {
            { glm::vec3(1.0f, 1.0f, 0.0f),      glm::vec2(1.0f, 1.0f),      glm::vec3(0.0f, 0.0f, 1.0f) },
            { glm::vec3(1.0f, -1.0f, 0.0f),     glm::vec2(1.0f, 0.0f),      glm::vec3(0.0f, 0.0f, 1.0f) },
            { glm::vec3(-1.0f, -1.0f, 0.0f),    glm::vec2(0.0f, 0.0f),      glm::vec3(0.0f, 0.0f, 1.0f) },
            { glm::vec3(-1.0f, 1.0f, 0.0f),     glm::vec2(0.0f, 1.0f),      glm::vec3(0.0f, 0.0f, 1.0f) },
            },
            { 0, 1, 2,      0, 2, 3 }
        };
    };

    struct MeshRenderer {
        MeshRenderer() = default;
        ~MeshRenderer() = default;
        static constexpr uint32_t getId() { return 2; }

        GlPipeline::VertexData vertex_data { {}, {} };
        GlPipeline::Material* material { nullptr };
    };

    // Note: Transform Comp with Camera:
    // - position   = position
    // - rotation   = forward
    // - scale      = up
    struct Camera {
        Camera() = default;
        ~Camera() = default;
        static constexpr uint32_t getId() { return 3; }

        enum Projection {
            Projection_Orthographic,
            Projection_Perspective
        };

        float fov { 45.0f };
        float near_plane { 0.0f };
        float far_plane { 100.0f };
        float ortho_scale { 100.0f };
        Projection projection { Projection_Orthographic };
        glm::mat4 projection_matrix { glm::mat4(1.0f) };
    };
}

#endif // __ONIP_COMPONENTS_GRAPHICS_COMPS_HPP__