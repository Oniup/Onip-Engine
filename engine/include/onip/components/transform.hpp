#ifndef __ONIP_COMPONENT_TRANSFORM_HPP__
#define __ONIP_COMPONENT_TRANSFORM_HPP__

#include "onip/utils/utils.hpp"

#include <glm/glm.hpp>

namespace onip {
    struct Transform {
        Transform() = default;
        ~Transform() = default;
        static constexpr uint32_t getId() { return 0; }

        glm::vec3 position { glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 scale { glm::vec3(1.0f, 1.0f, 1.0f) };
        glm::vec4 rotation { glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) };
    };

}

#endif // __ONIP_COMPONENT_TRANSFORM_HPP__