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
 * @see Scene, SparseSet,
 */
class EntityManager {
    typedef std::unordered_map<std::type_index, std::unique_ptr<ISparseSet> > ComponentSets;

    /// Next available entity ID
    Entity          m_nextEntity = 1;
    /// Storage for all component sets, indexed by their type
    ComponentSets   m_componentSets = {};

public:
    EntityManager() = default;

    /**
     * @brief Creates a new entity and returns its ID.
     * @return The ID of the created entity.
     */
    Entity  createEntity();

    /**
     * @brief Destroys an entity and removes all its associated components.
     * @param e The ID of the entity to destroy
     */
    void    destroyEntity(Entity e);

    /**
     * @brief Adds a component of the specified type to the given entity.
     * @tparam Component Type of the component to add
     * @param e The ID of the entity to which to add the component
     * @param args Arguments taken by the component to be created
     * @return A pointer to the added component
     */
    template<ComponentConcept Component, typename... Args>
    Component*  addComponent(Entity e, Args&& ...args) {
        SparseSet<Component> &sparseSet = getSparseSet<Component>();

        return sparseSet.addComponent(e, Component{ std::forward<Args>(args)...});
    }

    /**
     * @brief Retrieves a pointer to the component of the specified type associated with the given entity.
     * @tparam Component Type of the component to retrieve
     * @param e The ID of the entity for which to retrieve the component
     * @return A pointer to the requested component, or nullptr if the entity does not have the component
     */
    template<ComponentConcept Component>
    Component*  getComponent(Entity e) {
        SparseSet<Component> &sparseSet = getSparseSet<Component>();

        return sparseSet.getComponent(e);
    }

    /**
     * @brief Removes the component of the specified type from the given entity.
     * @tparam Component Type of the component to remove
     * @param e The ID of the entity for which to remove the component
     */
    template<ComponentConcept Component>
    void    removeComponent(Entity e) {
        SparseSet<Component> &sparseSet = getSparseSet<Component>();

        sparseSet.removeComponent(e);
    }

    /**
     * @brief Checks if the given entity has a component of the specified type.
     * @tparam Component Type of the component to check
     * @param e The ID of the entity for which to check the component
     * @return true if the entity has the component, false otherwise
     */
    template<ComponentConcept Component>
    bool    hasComponent(Entity e) {
        SparseSet<Component> &sparseSet = getSparseSet<Component>();
        return sparseSet.hasComponent(e);
    }

    /**
     * @brief Retrieves the SparseSet for the specified component type, creating it if it does not exist.
     * @tparam Component Type of the component for which to retrieve the SparseSet
     * @return A reference to the SparseSet for the specified component type
     */
    template<ComponentConcept Component>
    SparseSet<Component>&   getSparseSet() {
        const auto typeIndex = std::type_index(typeid(Component));

        if (!m_componentSets.contains(typeIndex)) {
            m_componentSets[typeIndex] = std::make_unique<SparseSet<Component> >();
        }

        return static_cast<SparseSet<Component> &>(*m_componentSets[typeIndex]);
    }
};
