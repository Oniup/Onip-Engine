#ifndef __ONIP_COMPONENTS_CAMERA_HPP__
#define __ONIP_COMPONENTS_CAMERA_HPP__

#include "onip/utils/utils.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace onip {
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

#endif // __ONIP_COMPONENTS_CAMERA_HPP__