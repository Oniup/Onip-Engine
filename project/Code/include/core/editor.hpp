#ifndef __ONIP_EDITOR_CORE_EDITOR_HPP__
#define __ONIP_EDITOR_CORE_EDITOR_HPP__

#include <onip/core/application.hpp>

// Next Component ID Available: 3

class Editor : public onip::Application {
public:
    Editor();
    ~Editor();

    void initialize_layers() override;
};

#endif // __ONIP_EDITOR_CORE_EDITOR_HPP__