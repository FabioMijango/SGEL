#pragma once
#include <ranges>
#include <unordered_map>
#include <string>
#include <SDL3/SDL_render.h>

#include "Animation.hpp"
#include "Engine.hpp"
#include "SDL3_image/SDL_image.h"

/**
 * @class Assets
 * @brief Manages game assets including textures and animations.
 *
 * Singleton pattern. Loads and caches SDL textures and animations from disk.
 * Provides access to loaded assets and handles their memory cleanup.
 *
 * @see Engine, Animation, Sprite
 */
class Assets {
    typedef std::unordered_map<std::string, SDL_Texture*> TextureMap;
    typedef std::unordered_map<std::string, Animation> AnimationMap;

    /// Map of texture IDs to SDL_Texture pointers
    TextureMap      m_textureMap;
    /// Map of animation IDs to Animation objects
    AnimationMap    m_animationMap;
    // TODO: Font support pending SDL_ttf integration
    // std::map<std::string, Fonts>      m_fontMap;

public:
    friend class Engine;

    /**
     * @brief Gets the singleton instance of the Assets manager.
     * @return Reference to the Assets singleton.
     */
    static Assets&      Instance();

    /**
     * @brief Loads assets from a configuration file.
     * @param path Path to the asset configuration file.
     * @note Not implemented yet.
     */
    void                loadFromFile(const std::string& path);

    /**
     * @brief Loads a texture from file and caches it.
     * @param textureID Unique identifier for the texture.
     * @param texturePath File path to the texture image.
     * @param smooth Enable linear filtering (true) or nearest neighbor (false).
     * @return Pointer to SDL_Texture, or nullptr on failure.
     */
    SDL_Texture*        loadTexture(const std::string &textureID, const std::string &texturePath, bool smooth);

    /**
     * @brief Loads a static sprite animation (single frame).
     * @param animationID Unique identifier for the animation.
     * @param textureID ID of the texture to use.
     * @param spriteRect Rectangle defining the sprite region.
     * @return Pointer to the cached Animation, or nullptr on failure.
     */
    Animation*          loadAnimation(const std::string& animationID, const std::string& textureID, SDL_FRect spriteRect);

    /**
     * @brief Loads an animated sprite with multiple frames.
     * @param animationID Unique identifier for the animation.
     * @param textureID ID of the texture containing animation frames.
     * @param animRect Rectangle containing all animation frames.
     * @param frameCount Number of frames in the animation.
     * @param speed Frames to display per animation frame (higher = slower).
     * @return Pointer to the cached Animation, or nullptr on failure.
     */
    Animation*          loadAnimation(const std::string &animationID, const std::string &textureID, SDL_FRect animRect,
                                      Uint32 frameCount, Uint32 speed);

    /**
     * @brief Retrieves a cached texture by ID.
     * @param textureName Texture identifier.
     * @return Pointer to SDL_Texture, or nullptr if not found.
     */
    SDL_Texture*        getTexture(const std::string &textureName) const;

    /**
     * @brief Retrieves a cached animation by ID.
     * @param animationName Animation identifier.
     * @return Reference to the Animation object.
     */
    const Animation&    getAnimation(const std::string &animationName) const;

    /**
     * @brief Gets all cached textures.
     * @return Reference to the texture map.
     */
    const TextureMap&   getTextureMap() const;

    /**
     * @brief Gets all cached animations.
     * @return Reference to the animation map.
     */
    const AnimationMap& getAnimationMap() const;

    /**
     * @brief Destroys and unloads all cached assets.
     */
    void destroyAllAssets();

    /**
     * @brief Unloads a specific texture. Also unload the animations associated to that texture.
     * @param textureID Texture identifier.
     * @return true if successful, false if not found.
     */
    bool                unloadTexture(const std::string &textureID);

    /**
     * @brief Unloads a specific animation.
     * @param animationID Animation identifier.
     * @return true if successful, false if not found.
     */
    bool                unloadAnimation(const std::string &animationID);
};
