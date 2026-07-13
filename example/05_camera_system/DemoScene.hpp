#pragma once
#include "EntityManager.hpp"
#include "Scene.hpp"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_render.h"

class DemoScene : public Scene {
    EntityManager entityManager;
    Entity player = -1;

    const float SPEED = 100.f;

public:
    bool init() override;
    SDL_AppResult update(float deltaTime) override;
    SDL_AppResult eventHandler(const SDL_Event *event) override;
    void sDoAction(const Action &action) override;
    void sRender(SDL_Renderer* renderer) override;
    void exit() override;

    std::shared_ptr<Scene> changeScene() override;
};
