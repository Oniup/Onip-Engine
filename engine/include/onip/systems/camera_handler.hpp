#ifndef __ONIP_SYSTEMS_CAMERA_HANDLER_HPP__
#define __ONIP_SYSTEMS_CAMERA_HANDLER_HPP__

#include "onip/utils/utils.hpp"
#include "onip/components/graphics_comps.hpp"
#include "onip/core/scene_manager.hpp"

#include <vector>

namespace onip {
    class CameraHandler : public CustomSystem {
    public:
        CameraHandler() = default;
        ~CameraHandler() = default;
        ONIP_INLINE const char* getName() override { return "Camera Handler"; }

        void onUpdate() override;
    private:
        std::vector<Camera*> m_cameras {};
    };
}

#endif // __ONIP_SYSTEMS_CAMERA_HANDLER_HPP__