#ifndef __ONIP_EDITOR_PANEL_HANDLER_HPP__
#define __ONIP_EDITOR_PANEL_HANDLER_HPP__

#include "onip/config.hpp"
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
            ONIP_INLINE bool* getOpenedPtr() { return &m_open; }
            ONIP_INLINE void setClose() { m_open = false; }
            ONIP_INLINE void setOpen() { m_open = true; }

            ONIP_INLINE ImGuiWindowFlags& getImGuiWindowFlags() { return m_window_flags; }
            ONIP_INLINE const ImGuiWindowFlags getImGuiWindowFlags() const { return m_window_flags; }

            ONIP_INLINE virtual const char* getName() const = 0;
            virtual void settingsBeforeImGuiBegin() {
                ImGuiWindowClass window_class;
                window_class.DockNodeFlagsOverrideSet;
                window_class.DockingAlwaysTabBar = true;
                ImGui::SetNextWindowClass(&window_class);
            }
            virtual void onImGuiDraw() = 0;
        private:
            bool m_open { true };
            ImGuiWindowFlags m_window_flags { Config::default_imgui_window_flags };
        };

        struct GlobalStyling {
            ImGuiStyle* style { nullptr };
            ImColor warning_color { ImColor(0.9f, 0.7f, 0.0f) };
            ImColor error_color { ImColor(1.0f, 0.2f, 0.0f) };
            float font_size { 0.0f };
            std::string font_path { "" };
        };

        PanelHandler();
        ~PanelHandler() override;

        GlobalStyling* getGlobalImGuiStyling() { return &m_global_styling; }
        const GlobalStyling* getGlobalImGuiStyling() const { return &m_global_styling; }

        void onUpdate() override;
        void updateGlobalStyle(const GlobalStyling& custom_style);
        void updateImGuiPlatformWindow();
        void enableMainWindowDockSpace(bool enable);

        template <typename _Panel, typename ... _Args>
        _Panel* addPanel(_Args ... args) {
            m_panels.emplace_back(std::move(new _Panel { args... }));
            return static_cast<_Panel*>(m_panels.back());
        }
        
        template <typename _Panel>
        bool removePanel() {
            return removePanel(_Panel::getName());
        }
        bool removePanel(const char* name);
    private:
        void glNewFrame();
        void glEndFrame();

        ImGuiIO* m_io { nullptr };
        std::vector<Panel*> m_panels {};
        GlobalStyling m_global_styling {};
    };
}

#endif // __ONIP_EDITOR_PANEL_HANDLER_HPP__