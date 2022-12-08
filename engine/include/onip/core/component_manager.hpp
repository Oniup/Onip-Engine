#ifndef __ONIP_CORE_COMPONENT_MANAGER_HPP__
#define __ONIP_CORE_COMPONENT_MANAGER_HPP__

#include "onip/utils/utils.hpp"
#include "onip/utils/pool.hpp"

#include <iostream>
#include <unordered_map>

namespace onip {
    struct ComponentMeta {
        size_t type;
    };
        
    class ComponentManager {
    public:
        ComponentManager() = default;
        ~ComponentManager() = default;

        void create_pool(size_t chunk_size) {
        }

        template <typename ... _Components>
        void push_component(uint32_t entity_id) {
            float index = get_pool_index<_Components...>();

            if (m_pools.find(index) != m_pools.end()) {
                size_t sizes[] = { sizeof(_Components)... };
                uint32_t count = sizeof(chunk_size) / sizeof(size_t);
                size_t chunk_size = 0;
                for (uint32_t i = 0; i < count; i++) {
                    chunk_sizes += sizes[i];
                }
                create_pool(chunk_size);
            }

            Pool* pool = m_pools.at(index);
            void* chunk = pool->allocate();
        }
    private:
        template <typename ... _Components>
        static float get_pool_index() {
           const uint32_t ids[] = { _Components::id()... };
           const uint32_t size = sizeof(ids) / sizeof(uint32_t);

            float index = 0.0f;
            for (uint32_t i = 0; i < size; i++) {
                index += static_cast<float>(ids[i]) * 0.5f;
            }
            return index;
        }

        std::unordered_map<float, Pool*> m_pools;
    };
}

#endif // __ONIP_CORE_COMPONENT_MANAGER_HPP__