#include "core/editor.hpp"

#include <onip/config.hpp>
#include <onip/core/scene_manager.hpp>
#include <onip/utils/pool.hpp>
#include <onip/components/graphics_comps.hpp>
#include <onip/graphics/gl_pipeline.hpp>
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

    Ecs::addCustomSystem<GraphicsVertexExtractionSystem>();

    Ecs::createComponentGroup<Camera, Transform, SpriteRenderer, MeshRenderer>();
    entity = Ecs::createEntity("Player");
    SpriteRenderer* sprite_renderer = Ecs::addComponent<SpriteRenderer>(entity);
    sprite_renderer->material = GlPipeline::getMaterial("Sprite Default");
    sprite_renderer->sprite = GlPipeline::createTexture("Box Test", "engine/assets/textures/test_crate.png");

    sprite_renderer->vertices.push_back({ glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 1.0f, -1.0f) });
    sprite_renderer->vertices.push_back({ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 1.0f, -1.0f) });
    sprite_renderer->vertices.push_back({ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, -1.0f) });
    sprite_renderer->vertices.push_back({ glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 1.0f, -1.0f) });

    entity = Ecs::createEntity("Main Camera");
    Ecs::addComponent<Camera>(entity);

    Ecs::debugPrintComponentGroups();
}