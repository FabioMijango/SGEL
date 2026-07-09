#include "DemoScene.hpp"

bool DemoScene::init() {
    registerAction(SDL_SCANCODE_ESCAPE, "ESCAPE");
    registerAction(SDL_SCANCODE_SPACE, "JUMP");
    registerAction(SDL_SCANCODE_A, "RIGHT");
    registerAction(SDL_SCANCODE_D, "LEFT");
    registerAction(SDL_SCANCODE_W, "UP");
    registerAction(SDL_SCANCODE_S, "DOWN");

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

    std::string msg = "Action registered: " + action.name + " (" + (action.state == Action::State::Pressed ? "Pressed" : "Released") + ")";
    m_actionsHistory.emplace_back(msg);
    if (m_actionsHistory.size() > 10) {
        m_actionsHistory.erase(m_actionsHistory.begin());
    }
}

void DemoScene::sRender(SDL_Renderer* renderer)  {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


    SDL_RenderDebugText(renderer, 10, 10, "Last 10 actions registered! Press W, A, S, D or Space");
    for (size_t i = 0; i < m_actionsHistory.size(); ++i) {
        std::string msg = "Action registered: " + m_actionsHistory[i];
        SDL_RenderDebugText(renderer, 60, 50 + static_cast<int>(i) * 20, msg.c_str());
    }

    SDL_RenderPresent(renderer);
}

void DemoScene::exit() {};

std::shared_ptr<Scene> DemoScene::changeScene() {
    return nullptr;
};
