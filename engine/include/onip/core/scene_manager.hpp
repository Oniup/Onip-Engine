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

        std::string name { "Empty Scene" };
        uint32_t scene_index { 0 };
        EntityManager entity_manager {};
        ComponentManager component_manager {};
    };

    class SceneManager : public ApplicationLayer {
    public:
        SceneManager();
        ~SceneManager();

        ONIP_INLINE static Scene* getActiveScene() { return Application::getSceneManager()->m_active_scene; }
        ONIP_INLINE static const std::vector<Scene*>& getLoadedScenes() { return Application::getSceneManager()->m_loaded_scenes; }

        ONIP_INLINE static void setActive(std::string_view scene_name) { Application::getSceneManager()->implSetActive(scene_name); }
        ONIP_INLINE static void loadEmpty() { Application::getSceneManager()->implLoadEmpty(); }
        ONIP_INLINE static void loadExisting(std::string_view scene_path) { Application::getSceneManager()->implLoadExisting(scene_path); }
        ONIP_INLINE static void serialize(Scene* scene) { Application::getSceneManager()->implSerialize(scene); }
    private:
        void implSetActive(std::string_view scene_name);
        void implLoadEmpty();
        void implLoadExisting(std::string_view scene_path);
        void implSerialize(Scene* scene);

        std::vector<Scene*> m_loaded_scenes {};
        Scene* m_active_scene { nullptr };
    };




    template <typename _Component>
    ONIP_INLINE static _Component* addComponent() {
        return nullptr;
    }

    template <typename ... _Components> 
    ONIP_INLINE static void createComponentGroup() { Application::getSceneManager()->getActiveScene()->component_manager.createGroup<_Components...>(); }

    template <typename ... _Components>
    ONIP_INLINE static class Pool* getComponentGroupPool() { return Application::getSceneManager()->getActiveScene()->component_manager.getPool<_Components...>(); }
}

#endif // __ONIP_CORE_getSceneManager_HPP__