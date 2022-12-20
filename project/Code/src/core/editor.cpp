#include "core/editor.hpp"

#include <onip/config.hpp>
#include <onip/core/scene_manager.hpp>
#include <onip/utils/pool.hpp>
#include <onip/components/graphics_comps.hpp>
#include <onip/systems/vertex_extraction.hpp>
#include <glm/glm.hpp>

using namespace onip;

static Entity* entity = nullptr;

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

void Editor::initializeRequirements() {
    GlPipeline::createRenderer(new GlBatchRenderer());
    SceneManager::loadEmpty();

    Ecs::addCustomSystem<GraphicsVerticesHandler>();

    Ecs::createComponentGroup<Camera, Transform, SpriteRenderer, MeshRenderer>();
    entity = Ecs::createEntity("Player");
    Ecs::addComponent<SpriteRenderer>(entity);
    Ecs::addComponent<Camera>(entity);

    Transform* transform = Ecs::getComponent<Transform>(entity);

    Ecs::debugPrintComponentGroups();
}