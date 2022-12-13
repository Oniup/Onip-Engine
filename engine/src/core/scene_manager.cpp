#include "onip/core/scene_manager.hpp"

namespace onip {

    SceneManager::SceneManager() 
        : ApplicationLayer("Scene Manager") { }

    SceneManager::~SceneManager() {
        for (Scene* scene : m_loaded_scenes) {
            for (CustomSystem* system : scene->systems) {
                delete system;
            }
            delete scene;
        }
    }

    void SceneManager::onUpdate() {
        for (CustomSystem* system : m_active_scene->systems) {
            system->onUpdate();
        }

        m_active_scene->component_manager.clearDestroyedBuffer();
        m_active_scene->entity_manager.clearDestroyedBuffer();
    }

    void SceneManager::implSetActive(std::string_view scene_name) {
        for (auto scene : m_loaded_scenes) {
            if (scene->name == scene_name) {
                m_active_scene = scene;
                return;
            }
        }
    }

    void SceneManager::implLoadEmpty() {
        m_loaded_scenes.push_back(new Scene());
        if (m_active_scene == nullptr) {
            m_active_scene = m_loaded_scenes.back();
        }
    }

    void SceneManager::implLoadExisting(std::string_view scene_path) {
        // TODO: design scene serialization first
    }

    void SceneManager::implSerialize(Scene* scene) {
        // TODO: design scene serialization first
    }
}