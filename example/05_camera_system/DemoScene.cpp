#include "DemoScene.hpp"

#include "CameraUtils.hpp"

bool DemoScene::init() {
    registerAction(SDL_SCANCODE_A, "RIGHT");
    registerAction(SDL_SCANCODE_D, "LEFT");
    registerAction(SDL_SCANCODE_W, "UP");
    registerAction(SDL_SCANCODE_S, "DOWN");

    registerAction(ScrollType::Vertical, "ZOOM");

    player = entityManager.createEntity();
    entityManager.addComponent<TransformComponent>(player, SDL_FPoint{ 1000.f, 1000.f }, SDL_FPoint{ 1.0f, 1.0f });
    entityManager.addComponent<CameraComponent>(player, SDL_FPoint{ 1000.f, 1000.f }, 0.5f, SDL_FPoint{ 800.0f, 600.0f });
    entityManager.addComponent<KinematicComponent>(player, SDL_FPoint{ 0.0f, 0.0f }, SDL_FPoint{ 0.0f, 0.0f });

    for (int i = 0; i < 30; ++i) {
        Entity entity = entityManager.createEntity();
        entityManager.addComponent<TransformComponent>(entity, SDL_FPoint{ static_cast<float>(rand() % 2000), static_cast<float>(rand() % 2000) }, SDL_FPoint{ 1.0f, 1.0f });
    }

    return true;
}

SDL_AppResult DemoScene::update(float deltaTime) {
    auto* transform = entityManager.getComponent<TransformComponent>(player);
    auto* kinematic = entityManager.getComponent<KinematicComponent>(player);
    auto* camera = entityManager.getComponent<CameraComponent>(player);

    transform->position.x += kinematic->velocity.x * deltaTime;
    transform->position.y += kinematic->velocity.y * deltaTime;
    camera->position = transform->position;

    return SDL_APP_CONTINUE;
}

void DemoScene::sDoAction(const Action &action) {
    auto* kinematic = entityManager.getComponent<KinematicComponent>(player);
    if (action.state == Action::State::Pressed) {
        if (action.name == "RIGHT") {
            kinematic->velocity.x += -SPEED;
        }
        if (action.name == "LEFT") {
            kinematic->velocity.x += SPEED;
        }
        if (action.name == "UP") {
            kinematic->velocity.y += -SPEED;
        }
        if (action.name == "DOWN") {
            kinematic->velocity.y += SPEED;
        }
    }
    else if (action.state == Action::State::Released) {
        if (action.name == "RIGHT") {
            kinematic->velocity.x += SPEED;
        }
        if (action.name == "LEFT") {
            kinematic->velocity.x += -SPEED;
        }
        if (action.name == "UP") {
            kinematic->velocity.y += SPEED;
        }
        if (action.name == "DOWN") {
            kinematic->velocity.y += -SPEED;
        }
    } else if (action.state == Action::State::Vertical_Scroll) {
        auto* camera = entityManager.getComponent<CameraComponent>(player);
        camera->zoom += action.y * 0.1f;
        if (camera->zoom < 0.1f) {
            camera->zoom = 0.1f;
        }
    }
}

void DemoScene::sRender(SDL_Renderer* renderer)  {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    auto* camera = entityManager.getComponent<CameraComponent>(player);
    const auto& transformComponents = entityManager.getSparseSet<TransformComponent>().getComponents();

    SDL_FRect rects[transformComponents.size()];
    for (std::size_t i = 0; i < transformComponents.size(); ++i) {
        auto& tComponent = transformComponents[i];

        SDL_FPoint screenPos = sCamera::worldToScreen(tComponent.position, *camera);
        float screenW = 50.0f * tComponent.scale.x * camera->zoom;
        float screenH = 50.0f * tComponent.scale.y * camera->zoom;
        const SDL_FRect rect{ screenPos.x, screenPos.y, screenW, screenH };

        rects[i] = rect;
    }

    SDL_RenderRects(renderer, rects, transformComponents.size());

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
