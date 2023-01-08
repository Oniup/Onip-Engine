#include "core/editor.hpp"

#include <onip/config.hpp>
#include <onip/core/scene_manager.hpp>
#include <onip/components/graphics_comps.hpp>
#include <imgui/imgui.h>

onip::Application* onip::Config::globalApplicationInstance() {
    static Editor editor;
    return &editor;
}

void onip::Config::defaultComponents(onip::Entity* entity) {
    onip::addComponent<onip::Transform>(entity);
}