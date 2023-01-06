#ifndef __ONIP_EDITOR_PANEL_INSPECTOR_HPP__
#define __ONIP_EDITOR_PANEL_INSPECTOR_HPP__

#include <onip/graphics/panel_handler.hpp>

class Inspector : public onip::PanelHandler::Panel {
public:
    Inspector();
    ~Inspector() override = default;

    ONIP_INLINE const char* getName() const override { return "Inspector"; }
    void onImGuiDraw() override;
};

#endif // __ONIP_EDITOR_PANEL_INSPECTOR_HPP__