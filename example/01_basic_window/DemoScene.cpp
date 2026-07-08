#include "DemoScene.hpp"

bool DemoScene::init() {
    registerAction(SDL_SCANCODE_SPACE, "Space");
    return true;
}

SDL_AppResult DemoScene::update(float deltaTime) {
    return SDL_APP_CONTINUE;
}

void DemoScene::sDoAction(const Action &action) {
    if (action.state == Action::State::Pressed) {
        SDL_Log("Action %s pressed", action.name.c_str());
    }
    else if (action.state == Action::State::Released) {
        SDL_Log("Action %s released", action.name.c_str());
    }
}

void DemoScene::sRender(SDL_Renderer* renderer)  {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDebugText(renderer, 10, 10, "DemoScene :D");

    SDL_RenderPresent(renderer);
}

void DemoScene::exit() {};

SDL_AppResult DemoScene::eventHandler(const SDL_Event *event)  {
    if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
        SDL_Log("Escape key pressed, quitting...");
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

std::shared_ptr<Scene> DemoScene::changeScene() {
    return nullptr;
};
