#pragma once
#include <cstddef>
#include <limits>
#include <vector>

#include "Types.hpp"

/**
 * @interface ISparseSet
 * @brief Interface for a sparse set of components.
 */
class ISparseSet {
public:
    virtual     ~ISparseSet() = default;
    virtual void removeComponent(Entity e) = 0;
};


/**
 * @class SparseSet
 * @brief A sparse set data structure for storing components associated with entities.
 * @tparam Component The type of component to store in the sparse set.
 */
template<ComponentConcept Component>
class SparseSet : public ISparseSet {
    /// Dense array of components
    std::vector<Component>   m_denseComponents;
    /// Dense array of entity keys corresponding to the components
    std::vector<Entity>      m_denseKeys;
    /// Array for the entities to index into the dense arrays
    std::vector<std::size_t> m_sparse;

    static constexpr std::size_t INVALID{std::numeric_limits<std::size_t>::max()};

public:
    SparseSet() = default;


    /**
     * @brief Adds a component for the given entity.
     * @param e Entity which will add the component.
     * @param component The component to add.
     * @return Pointer to the added component.
     */
    Component* addComponent(Entity e, const Component& component) {
        if (e >= m_sparse.size()) {
            m_sparse.resize(e + 1, INVALID);
        }

        m_denseComponents.push_back(component);
        m_denseKeys.push_back(e);
        m_sparse[e] = m_denseComponents.size() - 1;

        return &m_denseComponents.back();
    }

    /**
     * @brief Retrieves the component associated with the given entity.
     * @param e Entity which component will get
     * @return Pointer to the component, or nullptr if not found
     */
    Component*  getComponent(Entity e) {
        if (e >= m_sparse.size()) {
            return nullptr;
        }

        std::size_t index = m_sparse[e];
        if (index == INVALID) {
            return nullptr;
        }

        return &m_denseComponents[index];
    }

    /**
     * @brief Removes the component associated with the given entity.
     * @param e Entity which component will be removed.
     */
    void        removeComponent(Entity e) override {
        {
            if (e >= m_sparse.size()) {
                return;
            }

            auto index = m_sparse[e];
            if (index == INVALID) {
                return;
            }

            if (index != m_denseComponents.size() - 1) {
                m_denseComponents[index] = std::move(m_denseComponents.back());

                Entity last = m_denseKeys.back();
                m_denseKeys[index] = last;

                m_sparse[last] = index;
            }

            m_sparse[e] = INVALID;

            m_denseComponents.pop_back();
            m_denseKeys.pop_back();
        }
    }

    /**
     * @brief Checks if the given entity has a component.
     * @param e Entity to check.
     * @return true if the entity has a component, false otherwise.
     */
    bool        hasComponent(Entity e) const {
        return e < m_sparse.size() && m_sparse[e] != INVALID;
    }

    std::vector<Component>&   getComponents() {
        return m_denseComponents;
    }
    std::vector<Entity>&      getKeys() {
        return m_denseKeys;
    }
};
