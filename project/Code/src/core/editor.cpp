#include "core/editor.hpp"

#include <onip/config.hpp>

namespace onip {
    Application* Config::application() {
        static Editor editor;
        return &editor;
    }

    void Config::default_components(Entity* entity) {
        // Ecs::add_component<Transform>(entity);
    }
}

Editor::Editor() {
}

Editor::~Editor() {
}

void Editor::initialize_layers() {
}