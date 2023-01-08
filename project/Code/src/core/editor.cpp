#include "core/editor.hpp"

#include "panels/docking_space.hpp"
#include "panels/console.hpp"
#include "panels/hierarchy.hpp"
#include "panels/assets.hpp"
#include "panels/inspector.hpp"
#include "panels/viewport.hpp"

#include <onip/config.hpp>
#include <onip/core/scene_manager.hpp>
#include <onip/graphics/panel_handler.hpp>
#include <onip/utils/pool.hpp>
#include <onip/components/graphics_comps.hpp>
#include <onip/graphics/gl_pipeline.hpp>
#include <onip/systems/vertex_extraction.hpp>
#include <onip/core/input.hpp>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

static onip::Entity* entity = nullptr;

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
        onip::Entity* entity = onip::createEntity("Player");
        onip::SpriteRenderer* sprite_renderer = onip::addComponent<onip::SpriteRenderer>(entity);
        sprite_renderer->overlay_color = glm::vec4(0.2f, 0.5f, 0.6f, 0.1f);
        sprite_renderer->material = onip::GlPipeline::getMaterial("Sprite Default");
        sprite_renderer->sprite = onip::GlPipeline::getTexture("Box Test");
        sprite_renderer->render_layer = 20;
        m_transform = onip::getComponent<onip::Transform>(entity);
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
    panelHandlerImguiStyle();
    Application::getPanelHandler()->enableMainWindowDockSpace(true);
    Application::getPanelHandler()->addPanel<DockingSpace>();
    Application::getPanelHandler()->addPanel<Console>();
    Application::getPanelHandler()->addPanel<Hierarchy>();
    Application::getPanelHandler()->addPanel<Assets>();
    Application::getPanelHandler()->addPanel<Inspector>();
    Application::getPanelHandler()->addPanel<Viewport>();

    onip::Debug::logError("This is a test error");
    onip::Debug::logWarning("This is a test warning");

    onip::GlBatchRenderer* batch_renderer =  static_cast<onip::GlBatchRenderer*>(onip::GlPipeline::createRenderer(new onip::GlBatchRenderer()));
    onip::GlPipeline::createTexture("Box Test", "engine/assets/textures/test_crate.png");
    
    onip::SceneManager::loadEmpty();
    onip::createComponentGroup<onip::Transform, onip::SpriteRenderer, onip::MeshRenderer>();
    onip::createComponentGroup<onip::Camera>();

    onip::addCustomSystem<onip::GraphicsVertexExtractionSystem>(batch_renderer);
    onip::addCustomSystem<MovingSpriteTransformToLeft>();

    onip::Entity* entity = onip::createEntity("Other");
    onip::SpriteRenderer* sprite_renderer = onip::addComponent<onip::SpriteRenderer>(entity);
    sprite_renderer->material = onip::GlPipeline::getMaterial("Sprite Default");
    sprite_renderer->sprite = onip::GlPipeline::getTexture("Box Test");
    sprite_renderer->render_layer = 10;

    entity = onip::createEntity("Other");
    onip::Transform* transform = onip::getComponent<onip::Transform>(entity);
    transform->position = glm::vec3(5.0f, 10.0f, 0.0f);
    sprite_renderer = onip::addComponent<onip::SpriteRenderer>(entity);
    sprite_renderer->material = onip::GlPipeline::getMaterial("Sprite Default");
    sprite_renderer->sprite = onip::GlPipeline::getTexture("Box Test");
    sprite_renderer->render_layer = 10;

    entity = onip::createEntity("Main Camera");
    onip::addComponent<onip::Camera>(entity);
    transform = onip::getComponent<onip::Transform>(entity);
    transform->rotation = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    transform->scale = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Editor::panelHandlerImguiStyle() {
    // TODO: read custom settings from jason file
    onip::PanelHandler::GlobalStyling styling {};
    ImGuiStyle style {};
    styling.style = &style;
#ifndef __WIN32
    styling.font_size = 16.0f;
    styling.font_path = "C:\\Windows\\Fonts\\arial.ttf";
#endif // __WIN32

    style.WindowMenuButtonPosition = ImGuiDir_None;
    style.WindowMinSize = ImVec2(
        onip::GlPipeline::getWindow()->getSize().x * 0.05f,
        onip::GlPipeline::getWindow()->getSize().x * 0.05f
    );

    style.TabBorderSize = 0.0f;
    style.ChildBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 0.0f;
    style.WindowBorderSize = 1.0f;

    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(1.00f, 1.00f, 1.00f, 0.03f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.29f, 0.29f, 0.29f, 0.41f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.29f, 0.29f, 0.29f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.29f, 0.29f, 0.29f, 0.85f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.43f, 0.43f, 0.43f, 0.77f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.77f, 0.77f, 0.77f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.66f, 0.66f, 0.66f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.45f, 0.45f, 0.45f, 0.40f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.31f, 0.31f, 0.31f, 0.40f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.29f, 0.29f, 0.29f, 0.41f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.45f, 0.45f, 0.45f, 0.40f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.45f, 0.45f, 0.45f, 0.40f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.83f, 0.83f, 0.83f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.61f, 0.61f, 0.61f, 0.24f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.61f, 0.61f, 0.61f, 0.24f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    Application::getPanelHandler()->updateGlobalStyle(styling);
}