#ifndef __ONIP_CORE_COMPONENT_MANAGER_HPP__
#define __ONIP_CORE_COMPONENT_MANAGER_HPP__

#include "onip/utils/utils.hpp"
#include "onip/utils/pool.hpp"
#include "onip/core/entity_manager.hpp"

#include <iostream>
#include <vector>
#include <tuple>

namespace onip {
    struct ComponentMeta {
        uint32_t comp_id {};
        Entity* entity { nullptr };
        void (*on_destroy)(ComponentMeta* self);
    };

    class ComponentManager {
    private:
        struct ComponentGroup {
            std::vector<uint32_t> comp_ids {};
            Pool* pool { nullptr };
        };

        struct ComponentDestroyingData {
            ComponentMeta* meta { nullptr };
            Pool* pool { nullptr };
        };

        struct ComponentIter {
            uint32_t index;
            Pool* currentIter { nullptr };
        };
    public:
        ComponentManager() = default;

        ~ComponentManager() {
            for (ComponentGroup* group : m_groups) {
                delete group->pool;
                delete group;
            }
        }

        template <typename _Component>
        _Component* getCompFromMeta(ComponentMeta* meta) {
            Byte* byte_data = reinterpret_cast<Byte*>(meta);
            byte_data = byte_data + sizeof(ComponentMeta);
            return reinterpret_cast<_Component*>(byte_data);
        }

        ComponentMeta* getMetaFromComp(void* component) {
            Byte* byte_data = static_cast<Byte*>(component);
            byte_data = byte_data - sizeof(ComponentMeta);
            return reinterpret_cast<ComponentMeta*>(byte_data);
        }

        template <typename ... _Components>
        void createGroup() {
            uint32_t ids[] = { _Components::getId() ... };
            size_t ids_size = sizeof(ids) / sizeof(uint32_t);

#ifndef NDEBUG
            if (!checkRepeatingIds(ids, ids_size)) {
                return;
            }
#endif // NDEBUG

            ComponentGroup* group = new ComponentGroup{};
            for (size_t i = 0; i < ids_size; i++) {
                group->comp_ids.push_back(ids[i]);
            }
            size_t sizes[] = { sizeof(_Components) ... };
            size_t chunk_size = sizes[0];
            for (size_t i = 1; i < ids_size; i++) {
                if (sizes[i] > chunk_size) {
                    chunk_size = sizes[i];
                }
            }
            chunk_size += sizeof(ComponentMeta);
            group->pool = new Pool(chunk_size, chunk_size * ONIP_SCENE_COMP_POOL_BLOCK_COUNT);
            m_groups.push_back(std::move(group));
        }


        template <typename ... _Components>
        Pool* getPool() {
            uint32_t ids[] = { _Components::getId() ... };
            size_t ids_size = sizeof(ids) / sizeof(uint32_t);

#ifndef NDEBUG
            if (!checkRepeatingIds(ids, ids_size)) {
                return nullptr;
            }
#endif // NDEBUG

            uint32_t found = 0;
            for (ComponentGroup* group : m_groups) {
                if (group->comp_ids.size() == ids_size) {
                    for (size_t i = 0; i < ids_size; i++) {
                        for (uint32_t id : group->comp_ids) {
                            if (ids[i] == id) {
                                found++;
                                continue;
                            }
                        }
                    }
                    if (found == ids_size) {
                        return group->pool;
                    }
                }
            }
            return nullptr;
        }

        template <typename ... _Components>
        Pool* getPoolWhichContains() {
            std::vector<uint32_t> comp_ids = { _Components::getId() ... };
            return getPoolWhichContains(comp_ids);
        }

        Pool* getPoolWhichContains(const std::vector<uint32_t>& comp_ids) {
#ifndef NDEBUG
            if (!checkRepeatingIds(comp_ids.data(), comp_ids.size())) {
                return nullptr;
            }
#endif // NDEBUG

            uint32_t found = 0;
            for (ComponentGroup* group : m_groups) {
                for (size_t i = 0; i < comp_ids.size(); i++) {
                    for (uint32_t id : group->comp_ids) {
                        if (comp_ids[i] == id) {
                            found++;
                            continue;
                        }
                    }
                }
                if (found == comp_ids.size()) {
                    return group->pool;
                }
            }
            return nullptr;
        }

        template <typename _Component, typename ... _Args>
        _Component* addComponent(Entity* entity, _Args ... comp_constructor_args) {
            Pool* pool = getPoolWhichContains<_Component>();
            if (pool != nullptr) {
                ComponentMeta* meta = static_cast<ComponentMeta*>(pool->allocateData());
                meta->comp_id = _Component::getId();
                meta->entity = entity;
                meta->on_destroy = nullptr;
                _Component* comp = getCompFromMeta<_Component>(meta);

                new (comp) _Component { comp_constructor_args ... };
                entity->components.push_back({ _Component::getId(), static_cast<void*>(comp) });
                return comp;
            }
            return nullptr;
        }

        template <typename _Component>
        _Component* getComponent(Entity* entity) {
            if (doesEntityHaveCompId(entity, _Component::getId())) {
                ComponentMeta* meta = getComponentMeta(entity, _Component::getId());
                return getCompFromMeta<_Component>(meta);
            }
            return nullptr;
        }

        ComponentMeta* getComponentMeta(Entity* entity, uint32_t comp_id, Pool* target_pool = nullptr) {
            if (target_pool == nullptr) {
                target_pool = getPoolWhichContains(std::vector<uint32_t>(comp_id));;
            }
            if (doesEntityHaveCompId(entity, comp_id)) {
                if (target_pool->getAllocations() > 0) {
                    for (void* ptr : *target_pool) {
                        if (Pool::isNull(ptr)) {
                            continue;
                        }

                        ComponentMeta* meta = static_cast<ComponentMeta*>(ptr);
                        if (meta->comp_id == comp_id) {
                            if (meta->entity->uuid == entity->uuid) {
                                return meta;
                            }
                        }
                    }
                }
            }
            return nullptr;
        }

        template <typename _Component>
        void destroyComponent(_Component* component) {
            ComponentMeta* meta = getMetaFromComp(component);
            Pool* target_pool = getPoolWhichContains<_Component>();
            m_destroying.push_back(new ComponentDestroyingData { meta, target_pool });
        }

        template <typename _Component>
        ONIP_INLINE bool destroyComponent(Entity* entity) {
            return destroyComponent(entity, _Component::getId());
        }

        bool destroyComponent(Entity* entity, uint32_t component_id) {
            ComponentMeta* meta = nullptr;
            if (doesEntityHaveCompId(entity, component_id, meta)) {
                Pool* target_pool = getPoolWhichContains(std::vector<uint32_t>(component_id));
                ONIP_ASSERT_FMT(target_pool, "something seriously has gone wrong, somehow, you were able to add the component: Id: %u, to a pool that was never created, therefore cannot destroy component", component_id);
                m_destroying.push_back(ComponentDestroyingData { meta, target_pool });
                return true;
            }
            return false;
        }

        void clearDestroyedBuffer() {
            for (ComponentDestroyingData& destroying : m_destroying) {
                if (destroying.meta->on_destroy != nullptr) {
                    destroying.meta->on_destroy(destroying.meta);
                }
                // removing the component reference from the entity
                uint32_t target_index = 0;
                for (; target_index < destroying.meta->entity->components.size(); target_index++) {
                    if (destroying.meta->entity->components.at(target_index).comp_id == destroying.meta->comp_id) {
                        break;
                    }
                }
                destroying.meta->entity->components.erase(destroying.meta->entity->components.begin() + target_index);

                destroying.pool->releaseData(destroying.meta);
            }
            m_destroying.clear();
        }

        void debugPrintComponents() {
#ifndef NDEBUG
            std::cout << "Debug Print Components\n";
            uint32_t group_index = 0;
            for (ComponentGroup* group : m_groups) {
                std::cout  << "\n[" << group_index << "]: Component Group Ids: ";
                for (uint32_t& id : group->comp_ids) {
                    std::cout << id << ", ";
                }
                std::cout << "\n";

                uint32_t released_fround = 0;
                for (void* ptr : *group->pool) {
                    if (Pool::isNull(ptr)) {
                        if (released_fround == group->pool->getAllocationReleased()) {
                            break;
                        }
                        released_fround++;
                        continue;
                    }

                    ComponentMeta* meta = static_cast<ComponentMeta*>(ptr);
                    std::cout << "Id: " << meta->comp_id << "\tEntity UUID: " << meta->entity->uuid;
                    if (meta->entity->tag != nullptr) {
                        std::cout << "\tEntity Tag: " << *meta->entity->tag;
                    }
                    std::cout << "\n";
                }
                std::cout << "\n";
                group_index++;
            }
#endif // NDEBUG
        }
    private:
        bool checkRepeatingIds(const uint32_t ids[], size_t ids_size) {
            for (size_t i = 0; i < ids_size; i++) {
                for (size_t j = 0; j < ids_size; j++) {
                    if (i != j) {
                        if (ids[i] == ids[j]) {
                            std::cout << "cannot create component group as there is two ids that are the same: [ ";
                            for (size_t i = 0; i < ids_size; i++) {
                                std::cout << ids[i] << " ";
                            }
                            std::cout << "]\n";
                            return false;
                        }
                    }
                }
            }
            return true;
        }

        bool doesEntityHaveCompId(Entity* entity, uint32_t comp_id) {
            bool found = false;
            for (EntityComponentData& data : entity->components) {
                if (comp_id == data.comp_id) {
                    found = true;
                    break;
                }
            }
            return found;
        }

        bool doesEntityHaveCompId(Entity* entity, uint32_t comp_id, ComponentMeta*& meta) {
            bool found = false;
            for (EntityComponentData& data : entity->components) {
                if (comp_id == data.comp_id) {
                    Byte* byte_data = static_cast<Byte*>(data.data);
                    byte_data = byte_data - sizeof(ComponentMeta);
                    meta = reinterpret_cast<ComponentMeta*>(byte_data);

                    found = true;
                    break;
                }
            }
            return found;
        }

        std::vector<ComponentGroup*> m_groups {};
        std::vector<ComponentDestroyingData> m_destroying;
    };
}

#endif // __ONIP_CORE_COMPONENT_MANAGER_HPP__