#include "onip/core/application.hpp"
#include "onip/config.hpp"
#include "onip/core/input.hpp"
#include "onip/graphics/gl_pipeline.hpp"
#include "onip/core/scene_manager.hpp"

namespace onip {
    Application* Application::getInstance() {
        return Config::settingsApplication();
    }

    void Application::run() {
        m_pipeline = static_cast<GlPipeline*>(addLayer(new GlPipeline()));
        m_scene_manager = static_cast<SceneManager*>(addLayer(new SceneManager()));

        initializeRequirements();
        while (!GlPipeline::getWindow()->isClosing()) {
            Input::pollEvents();
            GlPipeline::getWindow()->clearScreen();
            for (ApplicationLayer* layer : m_layers) {
                layer->onUpdate();
            }
            GlPipeline::getWindow()->swapBuffers();
        }
        destroyLayers();
    }

    ApplicationLayer* Application::addLayer(ApplicationLayer* layer) {
        m_layers.insert(m_layers.begin(), layer);
        return m_layers[0];
    }

    void Application::destroyLayers() {
        if (m_layers.size() > 0) {
            for (ApplicationLayer* layer : m_layers) {
                delete layer;
            }
        }
    }

}