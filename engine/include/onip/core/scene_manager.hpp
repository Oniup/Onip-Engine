#ifndef __ONIP_CORE_SCENE_MANAGER_HPP__
#define __ONIP_CORE_SCENE_MANAGER_HPP__

#include "onip/utils/utils.hpp"
#include "onip/core/application.hpp"
#include "onip/core/application_layer.hpp"
#include "onip/core/component_manager.hpp"
#include "onip/core/entity_manager.hpp"

#include <vector>
#include <string>
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

        ONIP_INLINE static Scene* active_scene() { return Application::scene_manager()->m_active_scene; }
        ONIP_INLINE static const std::vector<Scene*>& loaded_scenes() { return Application::scene_manager()->m_loaded_scenes; }

        ONIP_INLINE static void set_active(std::string_view scene_name) { Application::scene_manager()->impl_set_active(scene_name); }
        ONIP_INLINE static void load_empty() { Application::scene_manager()->impl_load_empty(); }
        ONIP_INLINE static void load_existing(std::string_view scene_path) { Application::scene_manager()->impl_load_existing(scene_path); }
        ONIP_INLINE static void serialize(Scene* scene) { Application::scene_manager()->impl_serialize(scene); }
    private:
        void impl_set_active(std::string_view scene_name);
        void impl_load_empty();
        void impl_load_existing(std::string_view scene_path);
        void impl_serialize(Scene* scene);

        std::vector<Scene*> m_loaded_scenes;
        Scene* m_active_scene = nullptr;
    };
}

#endif // __ONIP_CORE_SCENE_MANAGER_HPP__