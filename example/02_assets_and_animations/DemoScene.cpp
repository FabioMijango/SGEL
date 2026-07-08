#include "DemoScene.hpp"


bool DemoScene::init() {
    Assets& assets = Assets::Instance();
    auto* TexPlayer = assets.loadTexture("TexPlayer", "./res/player.png", true);
    if (!TexPlayer) {
        std::string msg = "Failed to load texture. " + std::string(SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", msg.c_str(), nullptr);
        return false;
    }
    m_player.anim = assets.loadAnimation("AnimPlayer", "TexPlayer", { 0, 0, 384, 64}, 6, 30);
    m_player.position = { 100.f, 100.f };

    return true;
}

SDL_AppResult DemoScene::update(float deltaTime) {
    return SDL_APP_CONTINUE;
}

void DemoScene::sDoAction(const Action &action) {
}

void DemoScene::sRender(SDL_Renderer* renderer)  {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    Sprite sprite = m_player.anim->getSprite();
    SDL_RenderTexture(renderer, sprite.m_texture, &sprite.m_textureRect, nullptr);

    SDL_RenderDebugText(renderer, 10, 10, "Assets by 'Seliel the Shaper', in 'Mana Seed: Character Base'");

    SDL_RenderPresent(renderer);
}

void DemoScene::exit() {
}

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
