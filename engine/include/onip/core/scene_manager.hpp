#ifndef __ONIP_CORE_getSceneManager_HPP__
#define __ONIP_CORE_getSceneManager_HPP__

#include "onip/utils/utils.hpp"
#include "onip/core/application.hpp"
#include "onip/core/application_layer.hpp"
#include "onip/core/component_manager.hpp"
#include "onip/core/entity_manager.hpp"

#include <vector>
#include <string>
#include <memory>
#include <string_view>

namespace onip {
    struct Scene {
        Scene() = default;
        ~Scene() = default;

        std::string name = "Empty Scene";
        uint32_t scene_index = 0;
        EntityManager entity_manager;
        ComponentManager component_manager;
    };

    class SceneManager : public ApplicationLayer {
    public:
        SceneManager();
        ~SceneManager() = default;

        ONIP_INLINE static std::shared_ptr<Scene> active_scene() { return Application::getSceneManager()->m_active_scene; }
        ONIP_INLINE static const std::vector<std::shared_ptr<Scene>>& loaded_scenes() { return Application::getSceneManager()->m_loaded_scenes; }

        ONIP_INLINE static void setActive(std::string_view scene_name) { Application::getSceneManager()->implSetActive(scene_name); }
        ONIP_INLINE static void load_empty() { Application::getSceneManager()->implLoadEmpty(); }
        ONIP_INLINE static void loadExisting(std::string_view scene_path) { Application::getSceneManager()->implLoadExisting(scene_path); }
        ONIP_INLINE static void serialize(Scene* scene) { Application::getSceneManager()->implSerialize(scene); }
    private:
        void implSetActive(std::string_view scene_name);
        void implLoadEmpty();
        void implLoadExisting(std::string_view scene_path);
        void implSerialize(Scene* scene);

        std::vector<std::shared_ptr<Scene>> m_loaded_scenes;
        std::shared_ptr<Scene> m_active_scene = nullptr;
    };
}

#endif // __ONIP_CORE_getSceneManager_HPP__