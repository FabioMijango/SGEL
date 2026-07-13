#pragma once
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

#include "EntityManager.hpp"
#include "Scene.hpp"
#include "Types.hpp"

struct MyCustomComponent {
    SDL_FPoint position { 0.0f, 0.0f };
    SDL_FPoint velocity { 0.0f, 0.0f };
};

struct MyCustomTimeComponent {
    Uint64 startTime { 0 };
    Uint64 lifetime { 0 };
};

class DemoScene : public Scene {
    EntityManager entityManager;
    std::vector<Entity> entitiesToDelete;

    void sMoveEntities(float deltaTime);
    void sTimer();

    void sDeleteEntities();
public:
    bool init() override;
    SDL_AppResult update(float deltaTime) override;
    SDL_AppResult eventHandler(const SDL_Event *event) override;
    void sDoAction(const Action &action) override;
    void sRender(SDL_Renderer* renderer) override;
    void exit() override;

    std::shared_ptr<Scene> changeScene() override;
};
