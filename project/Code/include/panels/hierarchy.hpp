#ifndef __ONIP_EDITOR_PANELS_HIERARCHY_HPP__
#define __ONIP_EDITOR_PANELS_HIERARCHY_HPP__

#include <onip/graphics/panel_handler.hpp>

class Hierarchy : public onip::PanelHandler::Panel {
public: 
    Hierarchy();
    ~Hierarchy() override = default;

    ONIP_INLINE const char* getName() const override { return "Hierarchy"; }
    void onImGuiDraw() override;
};

#endif // __ONIP_EDITOR_PANELS_HIERARCHY_HPP__