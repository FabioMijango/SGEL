#pragma once
#include "EntityManager.hpp"
#include "Types.hpp"

namespace sPhysics {
    /**
     * @brief Get the bounding box of an entity based on its transform and collider components.
     * @param transform Transform component of the entity.
     * @param collider Collider component of the entity.
     * @return Bounding box rectangle.
     */
    inline SDL_FRect getBoundingBox(TransformComponent& transform, BoxColliderComponent& collider) {
        SDL_FRect boundingBox = {
            transform.position.x + collider.offset.x,
            transform.position.y + collider.offset.y,
            collider.size.x * transform.scale.x,
            collider.size.y * transform.scale.y
        };
        return boundingBox;
    }

    /**
     * @brief Check for collision between two axis-aligned bounding boxes (AABBs).
     * @param rectA First rectangle.
     * @param rectB Second rectangle.
     * @return True if the rectangles collide, false otherwise.
     */
    inline bool checkAABBCollision(SDL_FRect rectA, SDL_FRect rectB) {
        return rectA.x < rectB.x + rectB.w &&
               rectA.x + rectA.w > rectB.x &&
               rectA.y < rectB.y + rectB.h &&
               rectA.y + rectA.h > rectB.y;
    }

    /**
     * @brief Check for collision between a point and an axis-aligned bounding box (AABB).
     * @param point Point to check.
     * @param rect Rectangle to check.
     * @return True if the point is inside the rectangle, false otherwise.
     */
    inline bool checkPointAABBCollision(SDL_Point point, SDL_FRect rect) {
        return point.x >= rect.x && point.x <= rect.x + rect.w &&
               point.y >= rect.y && point.y <= rect.y + rect.h;
    }

}
