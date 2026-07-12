#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "SparseSet.hpp"

/**
 * @class EntityManager
 * @brief Manages entities and their components in an ECS architecture.
 *
 *  Handle the creation of entities and the addition/retrieval of components associated with those entities.
 *  Each component type has its own storage, allowing individual access and management of every entity data.
 *
 * @see Scene, SparseSet
 */
class EntityManager {
    typedef std::unordered_map<std::type_index, std::unique_ptr<ISparseSet> > ComponentSets;

    Entity          m_nextEntity = 1;
    ComponentSets   m_componentSets = {};

public:
    EntityManager() = default;

    Entity  createEntity();
    void    destroyEntity(Entity e);

    template<ComponentConcept Component, typename... Args>
    Component*  addComponent(Entity e, Args&& ...args) {
        SparseSet<Component> &sparseSet = getSparseSet<Component>();

        return sparseSet.addComponent(e, Component{ std::forward<Args>(args)...});
    }

    template<ComponentConcept Component>
    Component*  getComponent(Entity e) {
        SparseSet<Component> &sparseSet = getSparseSet<Component>();

        return sparseSet.getComponent(e);
    }

    template<ComponentConcept Component>
    void    removeComponent(Entity e) {
        SparseSet<Component> &sparseSet = getSparseSet<Component>();

        sparseSet.removeComponent(e);
    }

    template<ComponentConcept Component>
    bool    hasComponent(Entity e) {
        SparseSet<Component> &sparseSet = getSparseSet<Component>();
        return sparseSet.hasComponent(e);
    }

    template<ComponentConcept Component>
    SparseSet<Component>&   getSparseSet() {
        const auto typeIndex = std::type_index(typeid(Component));

        if (!m_componentSets.contains(typeIndex)) {
            m_componentSets[typeIndex] = std::make_unique<SparseSet<Component> >();
        }

        return static_cast<SparseSet<Component> &>(*m_componentSets[typeIndex]);
    }
};
