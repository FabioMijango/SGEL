#pragma once
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

#include "Assets.hpp"
#include "Scene.hpp"

struct Player {
    SDL_FPoint position = { 0.f, 0.f };
    Animation* anim = nullptr;
};

class DemoScene : public Scene {
    Player m_player = {};
public:
    bool init() override;
    SDL_AppResult update(float deltaTime) override;
    SDL_AppResult eventHandler(const SDL_Event *event) override;
    void sDoAction(const Action &action) override;
    void sRender(SDL_Renderer* renderer) override;
    void exit() override;

    std::shared_ptr<Scene> changeScene() override;
};
