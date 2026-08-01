#pragma once
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <unordered_map>
#include <vector>
#include <SDL3/SDL_stdinc.h>

#include "CameraUtils.hpp"
#include "EntityManager.hpp"
#include "PhysicsUtils.hpp"
#include "Types.hpp"

struct CellCoord {
    int x, y;

    bool operator==(const CellCoord& other) const;
};

struct CellCoordHash {
    std::size_t operator()(const CellCoord& c) const noexcept;
};

/**
 * @class SpatialGrid
 * @brief A spatial grid for broad-phase collision detection and camera culling
 *
 * @see EntityManager
 */
class SpatialGrid {
    using VectPairEntity = std::vector<std::pair<Entity, Entity>>;
    using CellMap = std::unordered_map<CellCoord, std::vector<Entity>, CellCoordHash>;

    /// Size of the cells in the grid (px)
    float                   m_cellSize = 0.f;
    /// Map of cells to entities contained within them
    CellMap                 m_cells;
    /// Reference to the entity manager for accessing components
    EntityManager*          m_entityManager = nullptr;

    void                    insert(Entity entity, SDL_FRect& rect);
    std::vector<CellCoord>  getCellsInBounds(SDL_FRect& bounds) const;

    CellCoord               toCell(float x, float y) const;
    void                    addPairIfNew(VectPairEntity& pairs, std::unordered_map<Uint64, bool>& seen, Entity a, Entity b) const;
    Uint64                  getPairKey(Entity a, Entity b) const;
public:
    SpatialGrid(float cellSize, EntityManager* entityManager);

    /**
     * @brief Sets the size of the cells in the spatial grid.
     * @param cellSize Size of the cells in pixels.
     */
    void                    setCellSize(float cellSize);

    /**
     * @brief Gets the size of the cells in the spatial grid.
     * @return Size of the cells in pixels.
     */
    float                   getCellSize() const;

    /**
     * @brief Populates the spatial grid with entities based on their positions and sizes.
     * This method should be called each frame to update the grid with the current entity positions.
     */
    void                    populateMap();

    /**
     * @brief Retrieves a list of potential collision pairs from the spatial grid.
     * This method returns pairs of entities that are in proximity and may potentially collide.
     * @return Vector of pairs of entities
     */
    VectPairEntity          getPotentialCollisions() const;

    /**
     * @brief Retrieves a list of entities that are within the camera's view rectangle.
     * @param camera Reference of the camera component
     * @return Vector of entities that are within the camera's view rectangle
     */
    std::vector<Entity>     getEntitiesInSight(CameraComponent& camera) const;

    /**
     * @brief Retrieves a list of potential collision pairs between entities with specified component tags.
     * @tparam TagA The component tag for the first entity type.
     * @tparam TagB The component tag for the second entity type.
     * @return Vector of pairs of entities that may potentially collide
     */
    template<typename TagA, typename TagB>
    VectPairEntity getPotentialCollisionBetween() const {
        VectPairEntity pairs;
        std::unordered_map<Uint64, bool> seen;
        static const CellCoord neighbors[4] = {{1, 0}, {0, 1}, {1, 1}, {-1, 1}};

        for (auto& [coord, entitiesVec] : m_cells) {
            for (std::size_t i = 0; i < entitiesVec.size(); ++i) {
                for (std::size_t j = i + 1; j < entitiesVec.size(); ++j) {
                    Entity ea = entitiesVec[i], eb = entitiesVec[j];
                    bool matchAB = m_entityManager->hasComponent<TagA>(ea) && m_entityManager->hasComponent<TagB>(eb);
                    bool matchBA = m_entityManager->hasComponent<TagA>(eb) && m_entityManager->hasComponent<TagB>(ea);
                    if (matchAB || matchBA)
                        addPairIfNew(pairs, seen, ea, eb);
                }
            }

            for (const CellCoord& off : neighbors) {
                CellCoord n{coord.x + off.x, coord.y + off.y};
                auto it = m_cells.find(n);
                if (it == m_cells.end()) continue;

                for (Entity a : entitiesVec) {
                    for (Entity b : it->second) {
                        bool matchAB = m_entityManager->hasComponent<TagA>(a) && m_entityManager->hasComponent<TagB>(b);
                        bool matchBA = m_entityManager->hasComponent<TagA>(b) && m_entityManager->hasComponent<TagB>(a);
                        if (matchAB || matchBA)
                            addPairIfNew(pairs, seen, a, b);
                    }
                }
            }
        }

        return pairs;
    }
};
