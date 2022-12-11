#include "onip/core/scene_manager.hpp"

namespace onip {

    SceneManager::SceneManager() 
        : ApplicationLayer("Scene Manager") { }

    void SceneManager::implSetActive(std::string_view scene_name) {
        for (auto scene : m_loaded_scenes) {
            if (scene->name == scene_name) {
                m_active_scene = scene;
                return;
            }
        }
    }

    void SceneManager::implLoadEmpty() {
        m_loaded_scenes.push_back(std::make_shared<Scene>());
    }

    void SceneManager::implLoadExisting(std::string_view scene_path) {
        // TODO: design scene serialization first
    }

    void SceneManager::implSerialize(Scene* scene) {
        // TODO: design scene serialization first
    }
}