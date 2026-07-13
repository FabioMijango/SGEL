#include "EntityManager.hpp"

Entity EntityManager::createEntity() {
    return m_nextEntity++;
}

void EntityManager::destroyEntity(Entity e) {
    for (auto& [typeIndex, sparseSet] : m_componentSets ) {
        sparseSet->removeComponent(e);
    }
}