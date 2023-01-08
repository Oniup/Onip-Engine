#ifndef __ONIP_EDITOR_PANELS_VIEWPORT_HPP__
#define __ONIP_EDITOR_PANELS_VIEWPORT_HPP__

#include <onip/graphics/panel_handler.hpp>

class Viewport : public onip::PanelHandler::Panel {
public:
    Viewport() = default;
    ~Viewport() = default;

    ONIP_INLINE const char* getName() const override { return "Viewport"; }
    void onImGuiDraw() override;
};

#endif // __ONIP_EDITOR_PANELS_VIEWPORT_HPP__