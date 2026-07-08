#include "Assets.hpp"

#include <vector>

Assets& Assets::Instance() {
    static Assets instance;
    return instance;
}

void Assets::loadFromFile(const std::string& path) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Not implemented yet", nullptr);
    exit(-1);
}

SDL_Texture* Assets::loadTexture(const std::string& textureID, const std::string& texturePath, bool smooth = false) {
    const Engine& engine = Engine::Instance();

    SDL_Texture* texture = IMG_LoadTexture(engine.m_renderer, texturePath.c_str());
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s. %s", texturePath.c_str(), SDL_GetError());
        return nullptr;
    }
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    auto[iterator, inserted] = m_textureMap.insert({textureID, texture});
    return iterator->second;
}

Animation* Assets::loadAnimation(const std::string& animationID, const std::string& textureID, SDL_FRect spriteRect = {0,0,0,0}) {
    auto it = m_textureMap.find(textureID);
    if (it == m_textureMap.end()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Texture %s not exist", textureID.c_str());
        return nullptr;
    }

    SDL_Texture* texture = it->second;

    if (SDL_RectEmptyFloat(&spriteRect)) {
        auto[ x, y, w, h] = spriteRect;
        spriteRect = SDL_FRect{ x, y, static_cast<float>(texture->w), static_cast<float>(texture->h) };
    }

    Animation animation(animationID, texture, spriteRect);

    auto[iterator, inserted] = m_animationMap.insert({animationID, animation});
    if (!inserted) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Sprite %s already exists", animationID.c_str());
        return nullptr;
    }
    return &iterator->second;
}

Animation* Assets::loadAnimation(const std::string& animationID, const std::string& textureID, SDL_FRect animRect, Uint32 frameCount, Uint32 speed) {
    auto it = m_textureMap.find(textureID);
    if (it == m_textureMap.end()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Texture %s not exist", textureID.c_str());
        return nullptr;
    }

    SDL_Texture* texture = it->second;

    if (SDL_RectEmptyFloat(&animRect)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Animation rect is empty");
        return nullptr;
    }

    Animation animation(animationID, texture, animRect, frameCount, speed);

    auto[iterator, inserted] = m_animationMap.insert({animationID, animation});
    if (!inserted) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Animation %s already exists", animationID.c_str());
        return nullptr;
    }
    return &iterator->second;
}

SDL_Texture* Assets::getTexture(const std::string& textureName) const {
    return m_textureMap.at(textureName);
}

const Animation& Assets::getAnimation(const std::string& animationName) const {
    return m_animationMap.at(animationName);
}

const std::unordered_map<std::string, SDL_Texture*>& Assets::getTextureMap() const {
    return m_textureMap;
}

const std::unordered_map<std::string, Animation>& Assets::getAnimationMap() const {
    return m_animationMap;
}

void Assets::destroyAllAssets() {
    m_animationMap.clear();
    for (const auto texture: m_textureMap | std::views::values) {
        SDL_DestroyTexture(texture);
    }
    m_textureMap.clear();
}

bool Assets::unloadTexture(const std::string &textureID) {
    SDL_Texture* texture = getTexture(textureID);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Texture %s not exists", textureID.c_str());
        return false;
    }

    std::vector<std::string> animationsToRemove;
    for (const auto& [animID, animation]: m_animationMap) {
        if (animation.getTexture() == texture) {
            animationsToRemove.push_back(animID);
        }
    }
    for (const auto& animation: animationsToRemove) {
        unloadAnimation(animation);
    }
    m_textureMap.erase(textureID);

    return true;
}

bool Assets::unloadAnimation(const std::string &animationID) {
    auto it = m_animationMap.find(animationID);
    if (it == m_animationMap.end()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Animation %s not exists", animationID.c_str());
        return false;
    }

    m_animationMap.erase(it);

    return true;
}
