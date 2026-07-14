#pragma once
#include "EntityManager.hpp"
#include "Types.hpp"

namespace sPhysics {

    SDL_FRect getBoundingBox(TransformComponent& transform, BoxColliderComponent& collider) {
        SDL_FRect boundingBox = {
            transform.position.x + collider.offset.x,
            transform.position.y + collider.offset.y,
            collider.width * transform.scale.x,
            collider.height * transform.scale.y
        };
        return boundingBox;
    }

    bool checkAABBCollision(SDL_FRect rectA, SDL_FRect rectB) {
        return rectA.x < rectB.x + rectB.w &&
               rectA.x + rectA.w > rectB.x &&
               rectA.y < rectB.y + rectB.h &&
               rectA.y + rectA.h > rectB.y;
    }

    bool checkPointAABBCollision(SDL_Point point, SDL_FRect rect) {
        return point.x >= rect.x && point.x <= rect.x + rect.w &&
               point.y >= rect.y && point.y <= rect.y + rect.h;
    }

}
