#include "Engine.hpp"
#include "DemoScene.hpp"

int main() {
    Engine& engine = Engine::Instance();
    engine.init({800, 600}, 60, std::make_shared<DemoScene>());

    while (true) {
        SDL_Event event = {0};

        if (SDL_PollEvent(&event)) {
            if (engine.sUserInput(&event) != SDL_APP_CONTINUE) break;
        }

        if (engine.update() != SDL_APP_CONTINUE) break;
        engine.sRender();
    }

    engine.sQuit();
    return 0;
}
