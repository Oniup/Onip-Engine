#ifndef __ONIP_COMPONENTS_GRAPHICS_COMPS_HPP__
#define __ONIP_COMPONENTS_GRAPHICS_COMPS_HPP__

#include <glm/glm.hpp>
#include <vector>
#include <array>

namespace onip {
    struct Transform {
        Transform() = default;
        ~Transform() = default;
        static uint32_t getId() { return 0; }

        glm::vec3 position { glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 scale { glm::vec3(1.0f, 1.0f, 1.0f) };
        glm::vec4 rotation { glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) };
    };

    struct SpriteRenderer {
        SpriteRenderer() = default;
        ~SpriteRenderer() = default;
        static uint32_t getId() { return 1; }

        std::array<uint32_t, 4> vertices;
    };

    struct MeshRenderer {
        MeshRenderer() = default;
        ~MeshRenderer() {
            delete vertices;
        }
        static uint32_t getId() { return 2; }

        std::vector<uint32_t>* vertices { new std::vector<uint32_t>() };
    };

    struct Camera {
        Camera() = default;
        ~Camera() = default;
        static uint32_t getId() { return 3; }

        glm::vec3 up { glm::vec3(0.0f, 1.0f, 0.0f) };
        glm::vec3 forward { glm::vec3(0.0f, 0.0f, 1.0f) };
    };
}

#endif // __ONIP_COMPONENTS_GRAPHICS_COMPS_HPP__