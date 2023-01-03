#ifndef __ONIP_EDITOR_PANEL_HANDLER_HPP__
#define __ONIP_EDITOR_PANEL_HANDLER_HPP__

#include <onip/core/application_layer.hpp>
#include <vector>
#include <string_view>

class PanelHandler : public onip::ApplicationLayer {
public:
    class Panel {
    public:
        virtual ~Panel() = default;
        virtual const std::string_view getName() const = 0;

        virtual void onUpdate() {};
        virtual void onImGuiDraw() = 0;
    };

    PanelHandler();
    ~PanelHandler() override = default;

    template <typename _Panel, typename ... _Args> _Panel* addPanel(_Args ... args);
    template <typename _Panel> bool removePanel();
    bool removePanel(std::string_view name);

    void onUpdate() override;
private:
    std::vector<Panel> m_panels {};
};

template <typename _Panel, typename ... _Args>
_Panel* PanelHandler::addPanel(_Args ... args) {
    m_panels.emplace_back(new _Panel { args... });
}

template <typename _Panel>
bool PanelHandler::removePanel() {
    return removePanel(_Panel::getName());
}

#endif // __ONIP_EDITOR_PANEL_HANDLER_HPP__