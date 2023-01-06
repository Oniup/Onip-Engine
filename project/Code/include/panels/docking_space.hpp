#ifndef __ONIP_EDITOR_DOCKING_SPACE_HPP__
#define __ONIP_EDITOR_DOCKING_SPACE_HPP__

#include <onip/graphics/panel_handler.hpp>
#include <imgui/imgui.h>

class DockingSpace : public onip::PanelHandler::Panel {
public:
    DockingSpace();
    ~DockingSpace() override = default;
    ONIP_INLINE const char* getName() const override { return "Docking Space"; }

    void settingsBeforeImGuiBegin() override;
    void onImGuiDraw() override;
private:
    ImGuiDockNodeFlags flags { ImGuiDockNodeFlags_None };
};

#endif // __ONIP_EDITOR_DOCKING_SPACE_HPP__