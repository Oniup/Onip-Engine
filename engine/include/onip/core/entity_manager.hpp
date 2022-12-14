#ifndef __ONIP_CORE_ENTITY_MANAGER_HPP__
#define __ONIP_CORE_ENTITY_MANAGER_HPP__

#include "onip/config.hpp"
#include "onip/utils/utils.hpp"
#include "onip/utils/pool.hpp"

#include <vector>

namespace onip {
    struct EntityComponentData {
        uint32_t comp_id { };
        void* data { nullptr };
    };

    struct Entity {
        Entity() = default;
        ~Entity() = default;

        UUID uuid { Utils::randUint64() };
        std::string* tag = { nullptr };
        uint32_t layer = 0;
        bool is_dynamic { true };
        std::vector<EntityComponentData> components {};
    };

    class EntityManager {
    public:
        EntityManager() = default;
        ~EntityManager();

        Entity* createEntity(const char* tag, uint32_t layer, bool is_dynamic);
        void destroyEntity(Entity* entity);
        void clearDestroyedBuffer();
private:
        Pool* m_entities { new Pool(sizeof(Entity), ONIP_SCENE_ENIT_POOL_BLOCK_COUNT) };
        std::vector<Entity*> m_destroying_entities {};
    };
}

#endif // __ONIP_CORE_ENTITY_MANAGER_HPP__