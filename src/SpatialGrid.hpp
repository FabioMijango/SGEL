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
};