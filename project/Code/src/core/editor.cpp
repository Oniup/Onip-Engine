#include "core/editor.hpp"

#include <onip/config.hpp>

namespace onip {
    Application* Config::settingsApplication() {
        static Editor editor;
        return &editor;
    }

    void Config::settingDefaultComponents(Entity* entity) {
    }
}

Editor::Editor() {
}

Editor::~Editor() {
}

void Editor::initializeLayers() {
}