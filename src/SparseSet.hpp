#pragma once
#include <cstddef>
#include <limits>
#include <vector>

#include "Types.hpp"

class ISparseSet {
public:
    virtual     ~ISparseSet() = default;
    virtual void removeComponent(Entity e) = 0;
};

template<ComponentConcept Component>
class SparseSet : public ISparseSet {
    std::vector<Component>   m_denseComponents;
    std::vector<Entity>      m_denseKeys;
    std::vector<std::size_t> m_sparse;

    static constexpr std::size_t INVALID{std::numeric_limits<std::size_t>::max()};

public:
    SparseSet() = default;

    Component* addComponent(Entity e, const Component& component) {
        if (e >= m_sparse.size()) {
            m_sparse.resize(e + 1, INVALID);
        }

        m_denseComponents.push_back(component);
        m_denseKeys.push_back(e);
        m_sparse[e] = m_denseComponents.size() - 1;

        return &m_denseComponents.back();
    }

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
