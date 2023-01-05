#include "core/editor.hpp"
#include "panels/console.hpp"

#include <onip/config.hpp>
#include <onip/core/scene_manager.hpp>
#include <onip/graphics/panel_handler.hpp>
#include <onip/utils/pool.hpp>
#include <onip/components/graphics_comps.hpp>
#include <onip/graphics/gl_pipeline.hpp>
#include <onip/systems/vertex_extraction.hpp>
#include <onip/core/input.hpp>
#include <glm/glm.hpp>

static onip::Entity* entity = nullptr;

onip::Application* onip::Config::settingsApplication() {
    static Editor editor;
    return &editor;
}

void onip::Config::settingDefaultComponents(onip::Entity* entity) {
    onip::Ecs::addComponent<onip::Transform>(entity);
}

Editor::Editor() {
}

Editor::~Editor() {
}

class MovingSpriteTransformToLeft : public onip::CustomSystem {
public:
    MovingSpriteTransformToLeft() = default;
    ~MovingSpriteTransformToLeft() = default;
    ONIP_INLINE const char* getName() override { return "Moving Sprite Transform To Left"; }

    void onStart() override {
        onip::Entity* entity = onip::Ecs::createEntity("Player");
        onip::SpriteRenderer* sprite_renderer = onip::Ecs::addComponent<onip::SpriteRenderer>(entity);
        sprite_renderer->overlay_color = glm::vec4(0.2f, 0.5f, 0.6f, 0.1f);
        sprite_renderer->material = onip::GlPipeline::getMaterial("Sprite Default");
        sprite_renderer->sprite = onip::GlPipeline::getTexture("Box Test");
        sprite_renderer->render_layer = 20;
        m_transform = onip::Ecs::getComponent<onip::Transform>(entity);
    }

    void onUpdate() override {
        glm::vec3 direction = glm::vec3(0.0f);
        if (onip::Input::getKey(onip::Input::Keyboard_W)) {
            direction.y += 1.0f;
        }
        if (onip::Input::getKey(onip::Input::Keyboard_S)) {
            direction.y -= 1.0f;
        }
        if (onip::Input::getKey(onip::Input::Keyboard_A)) {
            direction.x -= 1.0f;
        }
        if (onip::Input::getKey(onip::Input::Keyboard_D)) {
            direction.x += 1.0f;
        }
        float mag = glm::length(direction);
        if (mag > 0.0f) {
            m_transform->position += glm::normalize(direction) * 5.0f * onip::Time::getDeltaTime();
        }
    }
private:
    onip::Transform* m_transform { nullptr };
};

void Editor::initializeRequirements() {
    Application::getPanelHandler()->enableMainWindowDockSpace(true);

    onip::GlBatchRenderer* batch_renderer =  static_cast<onip::GlBatchRenderer*>(onip::GlPipeline::createRenderer(new onip::GlBatchRenderer()));
    onip::GlPipeline::createTexture("Box Test", "engine/assets/textures/test_crate.png");
    
    onip::SceneManager::loadEmpty();
    onip::Ecs::createComponentGroup<onip::Transform, onip::SpriteRenderer, onip::MeshRenderer>();
    onip::Ecs::createComponentGroup<onip::Camera>();

    onip::Ecs::addCustomSystem<onip::GraphicsVertexExtractionSystem>(batch_renderer);
    onip::Ecs::addCustomSystem<MovingSpriteTransformToLeft>();

    onip::Entity* entity = onip::Ecs::createEntity("Other");
    onip::SpriteRenderer* sprite_renderer = onip::Ecs::addComponent<onip::SpriteRenderer>(entity);
    sprite_renderer->material = onip::GlPipeline::getMaterial("Sprite Default");
    sprite_renderer->sprite = onip::GlPipeline::getTexture("Box Test");
    sprite_renderer->render_layer = 10;

    entity = onip::Ecs::createEntity("Other");
    onip::Transform* transform = onip::Ecs::getComponent<onip::Transform>(entity);
    transform->position = glm::vec3(5.0f, 10.0f, 0.0f);
    sprite_renderer = onip::Ecs::addComponent<onip::SpriteRenderer>(entity);
    sprite_renderer->material = onip::GlPipeline::getMaterial("Sprite Default");
    sprite_renderer->sprite = onip::GlPipeline::getTexture("Box Test");
    sprite_renderer->render_layer = 10;

    entity = onip::Ecs::createEntity("Main Camera");
    onip::Ecs::addComponent<onip::Camera>(entity);
    transform = onip::Ecs::getComponent<onip::Transform>(entity);
    transform->rotation = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    transform->scale = glm::vec3(0.0f, 1.0f, 0.0f);

    onip::Ecs::debugPrintComponentGroups();
}