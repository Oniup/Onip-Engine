#include "core/editor.hpp"

#include <onip/config.hpp>
#include <onip/core/scene_manager.hpp>
#include <onip/utils/pool.hpp>
#include <onip/components/graphics_comps.hpp>
#include <onip/graphics/gl_pipeline.hpp>
#include <onip/systems/vertex_extraction.hpp>
#include <onip/systems/camera_handler.hpp>
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
    MovingSpriteTransformToLeft(Entity* entity) {
        m_transform = Ecs::getComponent<Transform>(entity);
    }
    ~MovingSpriteTransformToLeft() = default;
    ONIP_INLINE const char* getName() override { return "Moving Sprite Transform To Left"; }

    void onUpdate() override {
        m_transform->position.x += 0.00001f;
    }
private:
    Transform* m_transform { nullptr };
};

void Editor::initializeRequirements() {
    GlBatchRenderer* batch_renderer =  static_cast<GlBatchRenderer*>(GlPipeline::createRenderer(new GlBatchRenderer()));
    
    SceneManager::loadEmpty();

    Ecs::addCustomSystem<GraphicsVertexExtractionSystem>(batch_renderer);
    Ecs::addCustomSystem<CameraHandler>();

    Ecs::createComponentGroup<Camera, Transform, SpriteRenderer, MeshRenderer>();
    entity = Ecs::createEntity("Player");
    SpriteRenderer* sprite_renderer = Ecs::addComponent<SpriteRenderer>(entity);
    sprite_renderer->overlay_color = glm::vec4(0.2f, 0.5f, 0.6f, 1.0f);
    sprite_renderer->material = GlPipeline::getMaterial("Sprite Default");
    sprite_renderer->sprite = GlPipeline::createTexture("Box Test", "engine/assets/textures/test_crate.png");

    Ecs::addCustomSystem<MovingSpriteTransformToLeft>(entity);

    entity = Ecs::createEntity("Main Camera");
    Ecs::addComponent<Camera>(entity);

    Ecs::debugPrintComponentGroups();
}