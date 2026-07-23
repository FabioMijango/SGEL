#include "Engine.hpp"

Engine& Engine::Instance() {
    static Engine instance;
    return instance;
}

SDL_AppResult Engine::init(const SDL_Point& windowSize, const Uint32 fpsLimit, const std::shared_ptr<Scene>& initialScene, const std::string& windowTitle, SDL_WindowFlags windowFlags) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(windowTitle.c_str(), windowSize.x, windowSize.y,
    windowFlags, &m_window, &m_renderer)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(m_renderer, windowSize.x, windowSize.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    m_fpsManager = new FPSManager();
    if (!m_fpsManager->setFramerate(fpsLimit)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to set frame rate\nUse [30-200] range", nullptr);
        return SDL_APP_FAILURE;
    }

    m_scene = initialScene;
    if (!m_scene->init(nullptr)) {
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

    m_deltaTime = static_cast<float>(m_fpsManager->framerateDelay()) / 1000;

    return result;
}

void Engine::sRender() const {
    m_scene->sRender(m_renderer);
}

SDL_AppResult Engine::sUserInput(const SDL_Event* event) const {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    const auto& sceneActionMap = m_scene->getActionMap();

    auto dispatchAction = [&](InputKey key, Action::State state, float x = 0.0f, float y = 0.0f) {
        if (auto it = sceneActionMap.find(key); it != sceneActionMap.end()) {
            Action triggeredAction = it->second;
            triggeredAction.state = state;
            triggeredAction.x = x;
            triggeredAction.y = y;
            m_scene->sDoAction(triggeredAction);
        }
    };

    if (event->type == SDL_EVENT_MOUSE_MOTION) {
        InputKey searchKey = { InputType::MouseMotion, 0 };
        dispatchAction(searchKey, Action::State::Mouse_Motion, event->motion.x, event->motion.y);
    }
    else if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP) {
        if (event->key.repeat) return SDL_APP_CONTINUE;

        InputKey searchKey = { InputType::Keyboard, static_cast<Uint32>(event->key.scancode) };
        auto newState = (event->type == SDL_EVENT_KEY_DOWN) ? Action::State::Pressed : Action::State::Released;

        dispatchAction(searchKey, newState);
    }
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN || event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        InputKey searchKey = { InputType::MouseButton, static_cast<Uint32>(event->button.button) };
        auto newState = (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) ? Action::State::Pressed : Action::State::Released;

        dispatchAction(searchKey, newState, event->button.x, event->button.y);
    }
    else if (event->type == SDL_EVENT_MOUSE_WHEEL) {
        auto scrollType = event->wheel.y != 0 ? ScrollType::Vertical : ScrollType::Horizontal;
        auto scrollState = event->wheel.y != 0 ? Action::State::Vertical_Scroll : Action::State::Horizontal_Scroll;
        InputKey key = { InputType::MouseWheel, static_cast<Uint32>(scrollType) };
        dispatchAction( key, scrollState, event->wheel.integer_x, event->wheel.integer_y);
    }

    auto result = m_scene->eventHandler(event);

    return result;
}

void Engine::sQuit() const {
    delete m_fpsManager;
    Assets::Instance().destroyAllAssets();
}
