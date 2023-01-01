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

class MovingSpriteTransformToLeft : public CustomSystem {
public:
    MovingSpriteTransformToLeft() = default;
    ~MovingSpriteTransformToLeft() = default;
    ONIP_INLINE const char* getName() override { return "Moving Sprite Transform To Left"; }

    void onStart() override {
        Entity* entity = Ecs::createEntity("Player");
        SpriteRenderer* sprite_renderer = Ecs::addComponent<SpriteRenderer>(entity);
        sprite_renderer->overlay_color = glm::vec4(0.2f, 0.5f, 0.6f, 1.0f);
        sprite_renderer->material = GlPipeline::getMaterial("Sprite Default");
        sprite_renderer->sprite = GlPipeline::getTexture("Box Test");
        m_transform = Ecs::getComponent<Transform>(entity);
    }

    void onUpdate() override {
        m_transform->position.x += 1.0f * Time::getDeltaTime();
    }
private:
    Transform* m_transform { nullptr };
};

void Editor::initializeRequirements() {
    GlBatchRenderer* batch_renderer =  static_cast<GlBatchRenderer*>(GlPipeline::createRenderer(new GlBatchRenderer()));
    GlPipeline::createTexture("Box Test", "engine/assets/textures/test_crate.png");
    
    SceneManager::loadEmpty();
    Ecs::createComponentGroup<Transform, SpriteRenderer, MeshRenderer>();
    Ecs::createComponentGroup<Camera>();

    Ecs::addCustomSystem<GraphicsVertexExtractionSystem>(batch_renderer);
    Ecs::addCustomSystem<MovingSpriteTransformToLeft>();

    Entity* entity = Ecs::createEntity("Other");
    SpriteRenderer* sprite_renderer = Ecs::addComponent<SpriteRenderer>(entity);
    sprite_renderer->overlay_color = glm::vec4(0.7f, 0.5f, 0.2f, 1.0f);
    sprite_renderer->material = GlPipeline::getMaterial("Sprite Default");
    sprite_renderer->sprite = GlPipeline::getTexture("Box Test");

    entity = Ecs::createEntity("Main Camera");
    Ecs::addComponent<Camera>(entity);
    Transform* camera_transform = Ecs::getComponent<Transform>(entity);
    camera_transform->rotation = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    camera_transform->scale = glm::vec3(0.0f, 1.0f, 0.0f);

    Ecs::debugPrintComponentGroups();
}