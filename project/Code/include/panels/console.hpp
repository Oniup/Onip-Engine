#ifndef __ONIP_EDITOR_PANEL_CONSOLE_HPP__
#define __ONIP_EDITOR_PANEL_CONSOLE_HPP__

#include <onip/graphics/panel_handler.hpp>
#include <onip/core/debug.hpp>
#include <imgui/imgui.h>

class Console : public onip::PanelHandler::Panel {
public:
    Console();
    ~Console() override = default;
    ONIP_INLINE const char* getName() const override { return "Console"; }

    void onImGuiDraw() override;
private:
    static Console* s_instance;
    uint32_t m_print_message_flags { onip::DebugLogSeverity_Message | onip::DebugLogSeverity_Warning | onip::DebugLogSeverity_Error };
    ImVec4 warning_message_color { 0.9f, 0.7f, 0.0f, 1.0f };
    ImVec4 error_message_color { 1.0f, 0.2f, 0.0f, 1.0f };
};

#endif // __ONIP_EDITOR_PANEL_CONSOLE_HPP__