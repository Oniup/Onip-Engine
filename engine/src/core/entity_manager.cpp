#include "onip/core/entity_manager.hpp"

namespace onip {
    EntityManager::~EntityManager() {
        delete m_entities;
    }

    Entity* EntityManager::createEntity(const char* tag, uint32_t layer, bool is_dynamic) {
        Entity* entity = m_entities->allocateData<Entity>();
        entity->layer = layer;
        entity->is_dynamic = is_dynamic;
        if (tag != nullptr) {
            entity->tag = new std::string(tag);
        }
        else {
            entity->tag = nullptr;
        }

        Config::settingDefaultComponents(entity);
        return entity;
    }

    void EntityManager::destroyEntity(Entity* entity) {
        m_destroying_entities.push_back(entity);
    }
        
    void EntityManager::clearDestroyedBuffer() {
        for (Entity* entity : m_destroying_entities) {
            for (EntityComponentData* data : entity->components) {
                // TODO: destroy all the components attached to the entity ... 
                delete data;
            }
            entity->components.clear();
            if (entity->tag != nullptr) {
                delete entity->tag;
            }
            m_entities->releaseData(entity);
        }
    }
}