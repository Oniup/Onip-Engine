#include "core/editor.hpp"

#include <onip/config.hpp>
#include <onip/core/scene_manager.hpp>
#include <onip/utils/pool.hpp>
#include <onip/components/graphics_comps.hpp>
#include <glm/glm.hpp>

using namespace onip;

Application* Config::settingsApplication() {
    static Editor editor;
    return &editor;
}

void Config::settingDefaultComponents(Entity* entity) {
    Ecs::addComponent<Transform>(entity);
}

Editor::Editor() {
}

Editor::~Editor() {
}

void Editor::initializeLayers() {
    SceneManager::loadEmpty();

    Ecs::createComponentGroup<Camera, Transform, SpriteRenderer, MeshRenderer>();
    Entity* entity = Ecs::createEntity("Player");
    Ecs::addComponent<SpriteRenderer>(entity);
    Ecs::addComponent<Camera>(entity);

    for (EntityComponentData* data : entity->components) {
        std::cout << "id: " << data->comp_id << "\n";
    }
}