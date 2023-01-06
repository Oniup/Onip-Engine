#include "onip/core/application.hpp"
#include "onip/config.hpp"
#include "onip/core/input.hpp"

#include "onip/graphics/gl_pipeline.hpp"
#include "onip/core/scene_manager.hpp"
#include "onip/graphics/panel_handler.hpp"
#include "onip/core/debug.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace onip {
    Application* Application::getInstance() {
        return Config::settingsApplication();
    }

    void Application::quit() {
        GlPipeline::getWindow()->close();
    }

    void Application::run() {
        assert(glfwInit() && "failed to initialize glfw for some reason ...");

        m_pipeline = static_cast<GlPipeline*>(addLayer(new GlPipeline()));
        m_scene_manager = static_cast<SceneManager*>(addLayer(new SceneManager()));
        m_panel_handler = static_cast<PanelHandler*>(addLayer(new PanelHandler()));
        addLayer(new Debug());

        ImGuiIO& io = ImGui::GetIO(); (void)io;

        initializeRequirements();
        while (!GlPipeline::getWindow()->isClosing()) {
            Input::pollEvents();

            for (ApplicationLayer* layer : m_layers) {
                layer->onUpdate();
            }

            m_panel_handler->updateImGuiPlatformWindow();
            m_pipeline->getWindow()->swapBuffers();
            m_pipeline->getWindow()->clearScreen();
        }

        destroyLayers();
        glfwTerminate();
    }

    ApplicationLayer* Application::addLayer(ApplicationLayer* layer) {
        m_layers.insert(m_layers.begin(), layer);
        return m_layers[0];
    }

    const ApplicationLayer* Application::getLayer(std::string_view name) const {
        for (ApplicationLayer* layer : m_layers) {
            if (layer->getName() == name) {
                return layer;
            }
        }
        return nullptr;
    }

    ApplicationLayer* Application::getLayer(std::string_view name) {
        for (ApplicationLayer* layer : m_layers) {
            if (layer->getName() == name) {
                return layer;
            }
        }
        return nullptr;
    }

    void Application::destroyLayers() {
        if (m_layers.size() > 0) {
            for (ApplicationLayer* layer : m_layers) {
                delete layer;
            }
        }
    }

}