#ifndef __ONIP_EDITOR_PANEL_CONSOLE_HPP__
#define __ONIP_EDITOR_PANEL_CONSOLE_HPP__

#include <onip/graphics/panel_handler.hpp>
#include <onip/core/debug.hpp>

class Console : public onip::PanelHandler::Panel {
public:
    Console();
    ~Console() override = default;
    ONIP_INLINE const char* getName() const override { return "Console"; }

    void onImGuiDraw() override;
private:
    static Console* s_instance;
    uint32_t m_print_message_flags { onip::DebugLogSeverity_Message | onip::DebugLogSeverity_Warning | onip::DebugLogSeverity_Error };
};

#endif // __ONIP_EDITOR_PANEL_CONSOLE_HPP__