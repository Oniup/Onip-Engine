#ifndef __ONIP_CORE_getSceneManager_HPP__
#define __ONIP_CORE_getSceneManager_HPP__

#include "onip/utils/utils.hpp"
#include "onip/core/application.hpp"
#include "onip/core/application_layer.hpp"
#include "onip/core/component_manager.hpp"
#include "onip/core/entity_manager.hpp"

#include <vector>
#include <tuple>
#include <string>
#include <memory>
#include <string_view>

namespace onip {
    class CustomSystem {
    public:
        ONIP_INLINE virtual const char* getName() = 0;

        virtual void onStart() {};
        virtual void onUpdate() {};
        virtual void onFixedUpdate() {};
        virtual void onLateUpdate() {};
    };

    struct Scene {
        Scene() = default;
        ~Scene() = default;

        std::string name { "Empty Scene" };
        uint32_t scene_index { 0 };
        EntityManager entity_manager {};
        ComponentManager component_manager {};
        std::vector<CustomSystem*> systems {};
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

        void onUpdate() override;
    private:
        void implSetActive(std::string_view scene_name);
        void implLoadEmpty();
        void implLoadExisting(std::string_view scene_path);
        void implSerialize(Scene* scene);

        std::vector<Scene*> m_loaded_scenes {};
        Scene* m_active_scene { nullptr };
        bool m_changed_scene { false };
    };

    struct Ecs {
        template <typename _Component, typename ... _Args>
        ONIP_INLINE static _Component* addComponent(Entity* entity, _Args ... comp_constructor_args) { return Application::getSceneManager()->getActiveScene()->component_manager.addComponent<_Component>(entity, comp_constructor_args...); }
        template <typename ... _Components> 
        ONIP_INLINE static void createComponentGroup() { Application::getSceneManager()->getActiveScene()->component_manager.createGroup<_Components...>(); }

        template <typename _Component>
        ONIP_INLINE static bool destroyComponent(Entity* entity) { return Application::getSceneManager()->getActiveScene()->component_manager.destroyComponent<_Component>(entity); }
        template <typename _Component>
        ONIP_INLINE static void destroyComponent(_Component* component) { return Application::getSceneManager()->getActiveScene()->component_manager.destroyComponent<_Component>(component); }
        ONIP_INLINE static bool destroyComponent(Entity* entity, uint32_t component_id) { return Application::getSceneManager()->getActiveScene()->component_manager.destroyComponent(entity, component_id); }

        template <typename _Component>
        ONIP_INLINE static _Component* getComponent(Entity* entity) { return Application::getSceneManager()->getActiveScene()->component_manager.getComponent<_Component>(entity); }
        template <typename _Component>
        ONIP_INLINE static _Component* getComponentFromMeta(ComponentMeta* meta) { return Application::getSceneManager()->getActiveScene()->component_manager.getCompFromMeta<_Component>(meta); }
        ONIP_INLINE static ComponentMeta* getMetaFromComponent(void* component) { return Application::getSceneManager()->getActiveScene()->component_manager.getMetaFromComp(component); }
        template <typename ... _Components>
        ONIP_INLINE static class Pool* getComponentGroupPool() { return Application::getSceneManager()->getActiveScene()->component_manager.getPool<_Components...>(); }
        ONIP_INLINE static ComponentMeta* getComponentMeta(Entity* entity, uint32_t comp_id, class Pool* target_pool = nullptr) { return Application::getSceneManager()->getActiveScene()->component_manager.getComponentMeta(entity, comp_id, target_pool); }
        template <typename _Component>
        ONIP_INLINE static std::vector<_Component*> getAllOfComponent() { return Application::getSceneManager()->getActiveScene()->component_manager.getAll<_Component>(); }

        template <typename _Component>
        ONIP_INLINE static bool checkIfSameComponentType(ComponentMeta* meta) { return Application::getSceneManager()->getActiveScene()->component_manager.checkIfSameType<_Component>(meta); }

        ONIP_INLINE static void debugPrintComponentGroups() { return Application::getSceneManager()->getActiveScene()->component_manager.debugPrintComponents(); }

        template <typename ... _Components>
        ONIP_INLINE static class Pool* getPoolWhichContains() { return Application::getSceneManager()->getActiveScene()->component_manager.getPoolWhichContains<_Components...>(); }
        ONIP_INLINE static class Pool* getPoolWhichContains(std::vector<uint32_t> comp_ids) { return Application::getSceneManager()->getActiveScene()->component_manager.getPoolWhichContains(comp_ids); }

        ONIP_INLINE static Entity* createEntity(bool is_dynamic = true) { return Application::getSceneManager()->getActiveScene()->entity_manager.createEntity(nullptr, 0, is_dynamic); }
        ONIP_INLINE static Entity* createEntity(const char* tag, bool is_dynamic = true) { return Application::getSceneManager()->getActiveScene()->entity_manager.createEntity(tag, 0, is_dynamic); }
        ONIP_INLINE static Entity* createEntity(uint32_t layer, bool is_dynamic = true) { return Application::getSceneManager()->getActiveScene()->entity_manager.createEntity(nullptr, layer, is_dynamic); }
        ONIP_INLINE static Entity* createEntity(const char* tag, uint32_t layer, bool is_dynamic = true) { return Application::getSceneManager()->getActiveScene()->entity_manager.createEntity(tag, layer, is_dynamic); }

        template <typename _System, typename ... _Args>
        ONIP_INLINE static void addCustomSystem(_Args ... args) { 
            std::vector<CustomSystem*>& systems = Application::getSceneManager()->getActiveScene()->systems;
            systems.push_back(new _System { args... });
            systems.back()->onStart();
        };
    };
}

#endif // __ONIP_CORE_getSceneManager_HPP__