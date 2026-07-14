#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "DemoScene.hpp"
#include "Engine.hpp"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    Engine& engine = Engine::Instance();
    *appstate = &engine;

    SDL_AppResult result = engine.init({800, 600}, 60, std::make_shared<DemoScene>());

    return result;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    auto* engine = static_cast<Engine*>(appstate);

    const SDL_AppResult result = engine->sUserInput(event);

    return result;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    auto* engine = static_cast<Engine*>(appstate);

    SDL_AppResult result = engine->update();
    engine->sRender();

    return result;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    auto* engine = static_cast<Engine*>(appstate);
    engine->sQuit();
}