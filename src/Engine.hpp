#pragma once
#include <cstdlib>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

#include "Assets.hpp"
#include "FpsManager.hpp"
#include "Scene.hpp"

class Engine {
    SDL_Window*     m_window        = nullptr;
    SDL_Renderer*   m_renderer      = nullptr;
    FPSManager*     m_fpsManager    = nullptr;
    float           m_deltaTime     = 0;

    std::shared_ptr<Scene> m_scene;

    Engine() = default;

public:
    friend class Assets;

    static Engine&  Instance();

    SDL_AppResult   init(const SDL_Point &windowSize, Uint32 fpsLimit, const std::shared_ptr<Scene>& initialScene);
    SDL_AppResult   update();
    void            sRender() const;
    SDL_AppResult   sUserInput(const SDL_Event *event) const;
    void            sQuit() const;
};
