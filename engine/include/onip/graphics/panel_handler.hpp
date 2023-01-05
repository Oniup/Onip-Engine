#ifndef __ONIP_EDITOR_PANEL_HANDLER_HPP__
#define __ONIP_EDITOR_PANEL_HANDLER_HPP__

#include "onip/core/application_layer.hpp"
#include "onip/graphics/gl_pipeline.hpp"

#include <vector>
#include <string_view>
#include <imgui/imgui.h>

namespace onip {
    class PanelHandler : public onip::ApplicationLayer {
    public:
        class Panel {
        public:
            virtual ~Panel() = default;

            ONIP_INLINE const bool isOpened() const { return m_open; }
            ONIP_INLINE void setClose() { m_open = false; }
            ONIP_INLINE void setOpen() { m_open = true; }

            ONIP_INLINE ImGuiWindowFlags& getImGuiWindowFlags() { return m_window_flags; }
            ONIP_INLINE const ImGuiWindowFlags getImGuiWindowFlags() const { return m_window_flags; }

            ONIP_INLINE virtual const char* getName() const = 0;
            virtual void onImGuiDraw() = 0;
        private:
            bool m_open { true };
            ImGuiWindowFlags m_window_flags { ImGuiWindowFlags_NoBringToFrontOnFocus };
        };

        PanelHandler();
        ~PanelHandler() override;

        void onUpdate() override;
        void updateImGuiPlatformWindow();

        void enableMainWindowDockSpace(bool enable);
    private:
        void glNewFrame();
        void glEndFrame();

        ImGuiIO* m_io { nullptr };
    };
}

#endif // __ONIP_EDITOR_PANEL_HANDLER_HPP__