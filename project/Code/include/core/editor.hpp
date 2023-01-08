#ifndef __ONIP_EDITOR_CORE_EDITOR_HPP__
#define __ONIP_EDITOR_CORE_EDITOR_HPP__

#include <onip/core/application.hpp>

class Editor : public onip::Application {
public:
    Editor();
    ~Editor();

    void initializeRequirements() override;
private:
    void panelHandlerImguiStyle();
};

#endif // __ONIP_EDITOR_CORE_EDITOR_HPP__