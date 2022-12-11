#include "core/editor.hpp"

#include <onip/config.hpp>
#include <onip/core/scene_manager.hpp>
#include <onip/utils/pool.hpp>
#include <glm/glm.hpp>

namespace onip {
    Application* Config::settingsApplication() {
        static Editor editor;
        return &editor;
    }

    void Config::settingDefaultComponents(Entity* entity) {
    }
}

struct TransformTest {
    TransformTest() = default;

    TransformTest(glm::vec3 position)
        : position(position) {}

    ~TransformTest() = default;

    static uint32_t getId() { return 0; }

    glm::vec3 position {};
    glm::vec3 scale { 1.0f, 1.0f, 1.0f };
};

struct SpriteRenderer {
    static uint32_t getId() { return 1; }

    uint32_t texture_id;
    glm::vec4 overlay_color;
};

Editor::Editor() {
}

Editor::~Editor() {
}

void Editor::initializeLayers() {
    onip::SceneManager::loadEmpty();
    onip::createComponentGroup<TransformTest, SpriteRenderer>();
    TransformTest* test = onip::addComponent<TransformTest>(69, glm::vec3(69.0f, 420.0f, 489.0f));
    std::cout << "hopefully it worked" << "\n";
}