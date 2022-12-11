#include "core/editor.hpp"

#include <onip/config.hpp>
#include <onip/core/scene_manager.hpp>
#include <onip/utils/pool.hpp>
#include <glm/glm.hpp>

struct TransformTest {
    static uint32_t getId() { return 0; }

    glm::vec3 position;
    glm::vec3 scale;
};

struct SpriteRenderer {
    static uint32_t getId() { return 1; }

    uint32_t texture_id;
    glm::vec4 overlay_color;
};

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
    onip::SceneManager::loadEmpty();
    onip::createComponentGroup<TransformTest, SpriteRenderer>();
    onip::Pool* pool = onip::getComponentGroupPool<TransformTest, SpriteRenderer>();
    if (pool != nullptr) {
        std::cout << "yes" << "\n";
    }
}