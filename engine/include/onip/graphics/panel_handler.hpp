#ifndef __ONIP_EDITOR_PANEL_HANDLER_HPP__
#define __ONIP_EDITOR_PANEL_HANDLER_HPP__

#include "onip/core/application_layer.hpp"
#include "onip/graphics/gl_pipeline.hpp"

#include <vector>
#include <string_view>

struct ImGuiIO;

namespace onip {
    class PanelHandler : public onip::ApplicationLayer {
    public:
        PanelHandler();
        ~PanelHandler() override;

        void onUpdate() override;
        void updateImGuiPlatformWindow();
    private:
        ImGuiIO* m_io { nullptr };
    };
}

#endif // __ONIP_EDITOR_PANEL_HANDLER_HPP__