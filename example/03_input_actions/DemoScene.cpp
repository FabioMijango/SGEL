#include "DemoScene.hpp"

bool DemoScene::init() {
    // If you register a Mouse button input, it also retrieves Mouse position.
    // registerMouseMotion("MOUSE_MOTION");

    registerAction(SDL_SCANCODE_ESCAPE, "ESCAPE");
    registerAction(SDL_SCANCODE_SPACE, "JUMP");
    registerAction(SDL_SCANCODE_A, "RIGHT");
    registerAction(SDL_SCANCODE_D, "LEFT");
    registerAction(SDL_SCANCODE_W, "UP");
    registerAction(SDL_SCANCODE_S, "DOWN");

    registerAction(SDL_BUTTON_LEFT, "MOUSE_LEFT");
    registerAction(SDL_BUTTON_RIGHT, "MOUSE_RIGHT");
    registerAction(SDL_BUTTON_X1, "MOUSE_X1");

    registerAction(ScrollType::Horizontal, "SCROLL_HORIZONTAL");
    registerAction(ScrollType::Vertical, "SCROLL_VERTICAL");

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
    else if (action.state == Action::State::Horizontal_Scroll) {
        SDL_Log("Action horizontal scroll: %f", action.x);
    }
    else if (action.state == Action::State::Vertical_Scroll) {
        SDL_Log("Action vertical scroll: %f", action.y);
    }
    else if (action.state == Action::State::Mouse_Motion) {
        SDL_Log("Action mouse motion: %f, %f", action.x, action.y);
    }

    std::string msg = "Action registered: " + action.name + " (" + std::to_string(static_cast<int>(action.state)) + ")";
    m_actionsHistory.emplace_back(msg);
    if (m_actionsHistory.size() > 10) {
        m_actionsHistory.erase(m_actionsHistory.begin());
    }
}

void DemoScene::sRender(SDL_Renderer* renderer)  {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderDebugText(renderer, 10, 10, "Last 10 actions registered! Press some buttons, clicks or scroll.");
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
