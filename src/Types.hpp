#pragma once
#include <concepts>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_timer.h>


using Entity = Uint32;

template <typename T>
concept ComponentConcept = std::is_class_v<T> && std::is_standard_layout_v<T>;

struct TransformComponent {
    SDL_FPoint position { 0.0f, 0.0f };
    SDL_FPoint scale { 1.0f, 1.0f };
};

struct KinematicComponent {
    SDL_FPoint velocity { 0.0f, 0.0f };
    SDL_FPoint acceleration { 0.0f, 0.0f };
};

struct BoxColliderComponent {
    SDL_FPoint size { 0.0f, 0.0f };
    SDL_FPoint offset { 0.0f, 0.0f };

    // Uint32 collisionLayer { 0 };
    // Uint32 collisionMask { 0 };
};

struct CameraComponent {
    SDL_FPoint position { 0.0f, 0.0f };
    float zoom { 1.0f };

    SDL_FPoint viewport { 0.0f, 0.0f };
};

struct TimeComponent {
    Uint64 timestamp { 0 };
};