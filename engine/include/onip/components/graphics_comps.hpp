#ifndef __ONIP_COMPONENTS_GRAPHICS_COMPS_HPP__
#define __ONIP_COMPONENTS_GRAPHICS_COMPS_HPP__

#include <glm/glm.hpp>
#include <stdint.h>

namespace onip {
    struct Transform {
        static uint32_t getId() { return 0; }
        Transform() = default;
        ~Transform() = default;
    };

    struct SpriteRenderer {
        static uint32_t getId() { return 1; }
        SpriteRenderer() = default;
        ~SpriteRenderer() = default;
    };

    struct MeshRenderer {
        static uint32_t getId() { return 2; }
    };

    struct Camera {
        static uint32_t getId() { return 3; }
        Camera() = default;
        ~Camera() = default;
    };
}

#endif // __ONIP_COMPONENTS_GRAPHICS_COMPS_HPP__