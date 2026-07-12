#include "DemoScene.hpp"

#include <cmath>

bool DemoScene::init() {
    registerAction(SDL_SCANCODE_SPACE, "Space");
    SDL_srand(time(nullptr));

    // Creating 100 inicial rects
    for (int i = 0; i < 100; ++i) {
        auto e = entityManager.createEntity();
        SDL_FPoint randomVelocity = {SDL_sinf(SDL_rand(360)) * 100, SDL_cosf(SDL_rand(360)) * 100};
        entityManager.addComponent<MyCustomComponent>(e, SDL_FPoint{400, 300}, randomVelocity);
        entityManager.addComponent<MyCustomTimeComponent>(e, SDL_GetTicks(), static_cast<Uint64>(SDL_rand(5000) + 1000));
    }
    return true;
}

SDL_AppResult DemoScene::update(float deltaTime) {
    sDeleteEntities();

    sMoveEntities(deltaTime);
    sTimer();

    return SDL_APP_CONTINUE;
}

void DemoScene::sDoAction(const Action &action) {
    if (action.state == Action::State::Pressed) {
        if (action.name == "Space") {
            auto e = entityManager.createEntity();

            SDL_FPoint randomVelocity = {SDL_sinf(SDL_rand(360)) * 100, SDL_cosf(SDL_rand(360)) * 100};
            entityManager.addComponent<MyCustomComponent>(e, SDL_FPoint{400, 300}, randomVelocity);
            entityManager.addComponent<MyCustomTimeComponent>(e, SDL_GetTicks(), static_cast<Uint64>(SDL_rand(5000) + 1000));
        }
    }
    else if (action.state == Action::State::Released) {
    }
}

void DemoScene::sRender(SDL_Renderer* renderer)  {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDebugText(renderer, 10, 10, "Press SPACE to create an Entity with ramdon lifespan");

    auto& transformSet = entityManager.getSparseSet<MyCustomComponent>().getComponents();
    for (auto& transform : transformSet) {
        SDL_FRect rect = {transform.position.x - 10, transform.position.y - 10, 20, 20};
        SDL_RenderRect(renderer, &rect);
    }


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

void DemoScene::sMoveEntities(float deltaTime) {
    auto& transformDenseComponents = entityManager.getSparseSet<MyCustomComponent>().getComponents();
    for (auto& transform : transformDenseComponents) {
        transform.position.x += transform.velocity.x * deltaTime;
        transform.position.y += transform.velocity.y * deltaTime;
    }
}


void DemoScene::sTimer() {
    auto& timeSet = entityManager.getSparseSet<MyCustomTimeComponent>();

    auto& timeDenseComponents = timeSet.getComponents();
    auto& timeDenseKeys = timeSet.getKeys();

    const auto now = SDL_GetTicks();
    for (size_t i = 0; i < timeDenseComponents.size(); ++i) {
        const auto& component = timeDenseComponents[i];
        if (now - component.startTime <= component.lifetime) {
            continue;
        }

        Entity e = timeDenseKeys[i];
        entitiesToDelete.push_back(e);
    }
}

void DemoScene::sDeleteEntities() {
    for (auto e : entitiesToDelete) {
        entityManager.destroyEntity(e);
    }
    entitiesToDelete.clear();
}