#ifndef __ONIP_COMPONENTS_SPRITE_AND_MESH_HPP__
#define __ONIP_COMPONENTS_SPRITE_AND_MESH_HPP__

#include "onip/utils/utils.hpp"
#include "onip/graphics/gl_batch_renderer.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace onip {
    struct SpriteRenderer {
        SpriteRenderer() = default;
        ~SpriteRenderer() = default;
        static constexpr uint32_t getId() { return 1; }

        bool flip_x { false };
        bool flip_y { false };
        glm::vec4 overlay_color { glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
        uint32_t render_layer {};

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

        uint32_t render_layer {};
        GlPipeline::VertexData vertex_data { {}, {} };
        GlPipeline::Material* material { nullptr };
    };
}

#endif // __ONIP_COMPONENTS_SPRITE_AND_MESH_HPP__