#include "core/panel_handler.hpp"

PanelHandler::PanelHandler() : onip::ApplicationLayer("Panel Handler") {
}

bool PanelHandler::removePanel(std::string_view name) {
    for (std::vector<Panel>::iterator panel = m_panels.begin(); panel != m_panels.end(); panel++) {
        if (panel->getName() == name) {
            m_panels.erase(panel);
            return true;
        }
    }
    return false;
}

void PanelHandler::onUpdate() {

}