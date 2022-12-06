#include "onip/core/scene_manager.hpp"

namespace onip {

    SceneManager::SceneManager() 
        : ApplicationLayer("Scene Manager") { }

    void SceneManager::impl_set_active(std::string_view scene_name) {
        for (Scene* scene : m_loaded_scenes) {
            if (scene->name == scene_name) {
                m_active_scene = scene;
                return;
            }
        }
    }

    void SceneManager::impl_load_empty() {
        m_loaded_scenes.push_back(new Scene {});
    }

    void SceneManager::impl_load_existing(std::string_view scene_path) {
        // TODO: design scene serialization first
    }

    void SceneManager::impl_serialize(Scene* scene) {
        // TODO: design scene serialization first
    }
}