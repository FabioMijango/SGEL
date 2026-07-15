#pragma once
#include <SDL3/SDL_rect.h>

#include "Types.hpp"

namespace sCamera {
    /**
     * @brief Convert world coordinates to screen coordinates based on the camera's position and zoom.
     * @param worldPos World coordinates to convert.
     * @param camera Camera component containing position, zoom, and viewport information.
     * @return Screen coordinates.
     */
    SDL_FPoint worldToScreen(const SDL_FPoint& worldPos, CameraComponent camera) {
        return {
            (worldPos.x - camera.position.x) * camera.zoom + camera.viewport.x * 0.5f,
            (worldPos.y - camera.position.y) * camera.zoom + camera.viewport.y * 0.5f
        };
    }

    /**
     * @brief Convert screen coordinates to world coordinates based on the camera's position and zoom.
     * @param screenPos Screen coordinates to convert.
     * @param camera Camera component containing position, zoom, and viewport information.
     * @return World coordinates.
     */
    SDL_FPoint screenToWorld(const SDL_FPoint& screenPos, CameraComponent camera) {
        return {
            (screenPos.x - (camera.viewport.x * 0.5f)) / camera.zoom + camera.position.x,
            (screenPos.y - (camera.viewport.y * 0.5f)) / camera.zoom + camera.position.y
        };
    }

    /**
     * @brief Get the bounds of the camera's view rectangle.
     * @param camera Camera component containing position, zoom, and viewport information.
     * @return Rectangle bounds.
     */
    SDL_FRect getViewBounds(CameraComponent& camera) {
        float viewW = camera.viewport.x * camera.zoom;
        float viewH = camera.viewport.y * camera.zoom;

        return {
            camera.position.x - (viewW * 0.5f),
            camera.position.y - (viewH * 0.5f),
            viewW,
            viewH
        };
    }

}
