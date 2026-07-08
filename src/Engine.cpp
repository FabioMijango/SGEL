#include "Engine.hpp"

#include "SDL3_framerate.h"

Engine& Engine::Instance() {
    static Engine instance;
    return instance;
}

SDL_AppResult Engine::init(const SDL_Point& windowSize, const Uint32 fpsLimit, const std::shared_ptr<Scene>& initialScene) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("SDL3 App", windowSize.x, windowSize.y,
    SDL_WINDOW_RESIZABLE, &m_window, &m_renderer)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(m_renderer, windowSize.x, windowSize.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    m_fpsManager = new FPSmanager();
    SDL_initFramerate(m_fpsManager);
    if (SDL_setFramerate(m_fpsManager, fpsLimit) != 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to set frame rate\nUse [30-200] range", nullptr);
        return SDL_APP_FAILURE;
    }

    m_scene = initialScene;
    if (!m_scene->init()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to initialize first scene", nullptr);
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult Engine::update() {
    if (m_scene->hasEnded()) {
        m_scene->exit();
        m_scene = m_scene->changeScene();
    }
    SDL_AppResult result = m_scene->update(m_deltaTime);
    for (auto& animation: Assets::Instance().m_animationMap | std::views::values) {
        animation.update();
    }

    m_deltaTime = static_cast<float>(SDL_framerateDelay(m_fpsManager)) / 1000;

    return result;
}

void Engine::sRender() const {
    m_scene->sRender(m_renderer);
}

SDL_AppResult Engine::sUserInput(const SDL_Event* event) const {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP) {
        const auto& sceneActionMap = m_scene->getActionMap();
        if (const auto it = sceneActionMap.find(event->key.scancode); it != sceneActionMap.end()) {

            const Action::State actionState = event->type == SDL_EVENT_KEY_DOWN ? Action::State::Pressed : Action::State::Released;
            const Action action = {it->second, actionState};

            m_scene->sDoAction(action);
        }
    }

    auto result = m_scene->eventHandler(event);

    return result;
}

void Engine::sQuit() const {
    delete m_fpsManager;
    Assets::Instance().destroyAllAssets();
}
