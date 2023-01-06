#ifndef __ONIP_EDITOR_PANELS_ASSETS_HPP__
#define __ONIP_EDITOR_PANELS_ASSETS_HPP__

#include <onip/graphics/panel_handler.hpp>

class Assets : public onip::PanelHandler::Panel {
public:
    Assets();
    ~Assets() override = default;

    ONIP_INLINE const char* getName() const override { return "Assets"; }
    void onImGuiDraw() override;
};

#endif // __ONIP_EDITOR_PANELS_ASSETS_HPP__