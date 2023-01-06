#ifndef __ONIP_EDITOR_PANEL_CONSOLE_HPP__
#define __ONIP_EDITOR_PANEL_CONSOLE_HPP__

#include <onip/graphics/panel_handler.hpp>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>
#include <imgui/imgui.h>

class Console : public onip::PanelHandler::Panel {
public:
    enum Severity {
        Severity_Message    = 0x01,
        Severity_Warning    = 0x02,
        Severity_Error      = 0x04,
    };

    Console();
    ~Console() override = default;
    ONIP_INLINE const char* getName() const override { return "Console"; }

    static void logMessage(std::string_view message);
    static void logWarning(std::string_view warning);
    static void logError(std::string_view error);
    static void log(std::string_view log, Severity severity);

    void onImGuiDraw() override;
private:
    static Console* s_instance;
    uint32_t m_print_message_flags { Severity_Message | Severity_Warning | Severity_Error };
    ImVec4 warning_message_color { 0.9f, 0.7f, 0.0f, 1.0f };
    ImVec4 error_message_color { 1.0f, 0.2f, 0.0f, 1.0f };
    std::vector<std::tuple<std::string, Severity>> m_logs {};
};

#endif // __ONIP_EDITOR_PANEL_CONSOLE_HPP__