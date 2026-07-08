#pragma once
#include <ranges>
#include <unordered_map>
#include <string>
#include <SDL3/SDL_render.h>

#include "Animation.hpp"
#include "Engine.hpp"
#include "SDL3_image/SDL_image.h"

class Assets {
    typedef std::unordered_map<std::string, SDL_Texture*> TextureMap;
    typedef std::unordered_map<std::string, Animation> AnimationMap;

    TextureMap      m_textureMap;
    AnimationMap    m_animationMap;
    // std::map<std::string, Fonts>      m_fontMap; // TODO: Not supported. Need SDL_ttf

public:
    friend class Engine;
    static Assets&      Instance();

    void                loadFromFile(const std::string& path); // TODO: Not implemented
    SDL_Texture*        loadTexture(const std::string &textureID, const std::string &texturePath, bool smooth);
    Animation*          loadAnimation(const std::string& animationID, const std::string& textureID, SDL_FRect spriteRect);
    Animation*          loadAnimation(const std::string &animationID, const std::string &textureID, SDL_FRect animRect,
                                      Uint32 frameCount, Uint32 speed);

    SDL_Texture*        getTexture(const std::string &textureName) const;
    const Animation&    getAnimation(const std::string &animationName) const;

    const TextureMap&   getTextureMap() const;
    const AnimationMap& getAnimationMap() const;

    void destroyAllAssets();
    bool                unloadTexture(const std::string &textureID);
    bool                unloadAnimation(const std::string &animationID);
};
