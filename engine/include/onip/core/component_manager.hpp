#ifndef __ONIP_CORE_COMPONENT_MANAGER_HPP__
#define __ONIP_CORE_COMPONENT_MANAGER_HPP__

#include "onip/utils/utils.hpp"
#include "onip/utils/pool.hpp"

#include <iostream>
#include <vector>

namespace onip {
    struct ComponentMeta {
        uint32_t comp_id;
        uint32_t entity_id;
    };

    class ComponentManager {
    private:
        struct ComponentGroup {
            std::vector<uint32_t> comp_ids;
            Pool* pool;
        };
    public:
        ComponentManager() = default;

        ~ComponentManager() {
            for (ComponentGroup* group : groups) {
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
            groups.push_back(std::move(group));
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
            for (ComponentGroup* group : groups) {
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
            uint32_t ids[] = { _Components::getId() ... };
            size_t ids_size = sizeof(ids) / sizeof(uint32_t);

#ifndef NDEBUG
            if (!checkRepeatingIds(ids, ids_size)) {
                return nullptr;
            }
#endif // NDEBUG

            uint32_t found = 0;
            for (ComponentGroup* group : groups) {
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

            return nullptr;
        }

        template <typename _Component, typename ... Args>
        _Component* addComponent(uint32_t entity_id, Args ... comp_constructor_args) {
            Pool* pool = getPoolWhichContains<_Component>();
            if (pool != nullptr) {
                ComponentMeta* meta = static_cast<ComponentMeta*>(pool->allocateData());
                meta->comp_id = _Component::getId();
                meta->entity_id = entity_id;
                _Component* comp = getCompFromMeta<_Component>(meta);

                new (comp) _Component { comp_constructor_args ... };
                return comp;
            }
            return nullptr;
        }
    private:
        bool checkRepeatingIds(uint32_t ids[], size_t ids_size) {
            for (size_t i = 0; i < ids_size; i++) {
                for (size_t j = 0; j < ids_size; j++) {
                    if (i != j) {
                        if (ids[i] == ids[j]) {
                            std::cout << "cannot create component group as there is two ids that are the same\n";
                            return false;
                        }
                    }
                }
            }
            return true;
        }
        std::vector<ComponentGroup*> groups {};
    };
}

#endif // __ONIP_CORE_COMPONENT_MANAGER_HPP__