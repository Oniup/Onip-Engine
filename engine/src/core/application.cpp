#include "onip/core/application.hpp"
#include "onip/config.hpp"
#include "onip/core/input.hpp"
#include "onip/graphics/gl_pipeline.hpp"
#include "onip/core/scene_manager.hpp"

namespace onip {
    Application* Application::get() {
        return Config::application();
    }

    void Application::run() {
        m_pipeline = static_cast<GLPipeline*>(add_layer(new GLPipeline()));
        m_scene_manager = static_cast<SceneManager*>(add_layer(new SceneManager()));

        initialize_layers();
        while (!m_pipeline->window()->closing()) {
            Input::poll_events();
            for (ApplicationLayer* layer : m_layers) {
                layer->on_update();
            }
            m_pipeline->window()->clear_screen();
        }
        destroy_layers();
    }

    ApplicationLayer* Application::add_layer(ApplicationLayer* layer) {
        m_layers.insert(m_layers.begin(), layer);
        return m_layers[0];
    }

    void Application::destroy_layers() {
        if (m_layers.size() > 0) {
            for (ApplicationLayer* layer : m_layers) {
                delete layer;
            }
        }
    }

}