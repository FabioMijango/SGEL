#include "SpatialGrid.hpp"

bool CellCoord::operator==(const CellCoord &other) const {
    return x == other.x && y == other.y;
}

std::size_t CellCoordHash::operator()(const CellCoord &c) const noexcept {
    Uint64 ux = static_cast<Uint32>(c.x);
    Uint64 uy = static_cast<Uint32>(c.y);
    return static_cast<std::size_t>((ux << 32) ^ uy);
}

CellCoord SpatialGrid::toCell(float x, float y) const {
    return CellCoord{
        static_cast<int>(std::floor(x / m_cellSize)),
        static_cast<int>(std::floor(y / m_cellSize))
    };
}

SpatialGrid::SpatialGrid(float cellSize, EntityManager* entityManager)
: m_cellSize(cellSize),
m_entityManager(entityManager)
{
    if (m_entityManager == nullptr)
        throw std::invalid_argument("EntityManager cannot be null");
}

void SpatialGrid::setCellSize(float cellSize) {
    m_cellSize = cellSize;
}

float SpatialGrid::getCellSize() const {
    return m_cellSize;
}

void SpatialGrid::insert(Entity entity, SDL_FRect &rect) {
    auto cells = getCellsInBounds(rect);
    for (const auto& cell : cells) {
        m_cells[cell].push_back(entity);
    }
}

std::vector<CellCoord> SpatialGrid::getCellsInBounds(SDL_FRect &bounds) const {
    CellCoord minCell = toCell(bounds.x, bounds.y);
    CellCoord maxCell = toCell(bounds.x + bounds.w, bounds.y + bounds.h);

    std::vector<CellCoord> cells;
    cells.reserve(static_cast<size_t>(maxCell.x - minCell.x + 1) * static_cast<size_t>(maxCell.y - minCell.y + 1));
    for (int cx = minCell.x; cx <= maxCell.x; ++cx) {
        for (int cy = minCell.y; cy <= maxCell.y; ++cy) {
            cells.push_back(CellCoord{cx, cy});
        }
    }
    return cells;
}

void SpatialGrid::populateMap() {
    m_cells.clear();
    auto m_entitiesWithCollider = m_entityManager->getSparseSet<BoxColliderComponent>().getKeys();
    for (auto e : m_entitiesWithCollider) {
        auto* transform = m_entityManager->getComponent<TransformComponent>(e);
        auto* boxCollider = m_entityManager->getComponent<BoxColliderComponent>(e);
        if (!transform || !boxCollider) continue;

        auto bounds = sPhysics::getBoundingBox(*transform, *boxCollider);
        insert(e, bounds);
    }
}

SpatialGrid::VectPairEntity SpatialGrid::getPotentialCollisions() const {
    VectPairEntity pairs;
    std::unordered_map<Uint64, bool> seen;
    static const CellCoord neighbors[4] = {{1, 0}, {0, 1}, {1, 1}, {-1, 1}};

    for (auto& [coord, entitiesVec] : m_cells) {
        for (std::size_t i = 0; i < entitiesVec.size(); ++i) {
            for (std::size_t j = i + 1; j < entitiesVec.size(); ++j) {
                addPairIfNew(pairs, seen, entitiesVec[i], entitiesVec[j]);
            }
        }

        for (const CellCoord& off : neighbors) {
            CellCoord n{coord.x + off.x, coord.y + off.y};
            auto it = m_cells.find(n);
            if (it == m_cells.end()) continue;

            for (Entity a : entitiesVec) {
                for (Entity b : it->second) {
                    addPairIfNew(pairs, seen, a, b);
                }
            }
        }
    }

    return pairs;
}

void SpatialGrid::addPairIfNew(VectPairEntity &pairs, std::unordered_map<Uint64, bool> &seen, Entity a, Entity b) const {
    if (a == b) return;

    Uint64 key = getPairKey(a, b);
    if (seen.find(key) == seen.end()) {
        seen[key] = true;
        pairs.emplace_back(std::min(a, b), std::max(a, b));
    }
}

Uint64 SpatialGrid::getPairKey(Entity a, Entity b) const {
    if (a < b) std::swap(a, b);
    return (static_cast<Uint64>(a) << 32) | static_cast<Uint64>(b);
}

std::vector<Entity> SpatialGrid::getEntitiesInSight(CameraComponent& camera) const {
    std::vector<Entity> result;

    SDL_FRect cameraBounds = sCamera::getViewBounds(camera);
    std::vector<CellCoord> cells = getCellsInBounds(cameraBounds);

    for (auto& coord : cells) {
        auto it = m_cells.find(coord);
        if (it == m_cells.end())
            continue;

        for (auto& entity : it->second) {
            result.push_back(entity);
        }
    }

    std::ranges::sort(result);
    result.erase(std::ranges::unique(result).begin(), result.end());
    return result;
}
