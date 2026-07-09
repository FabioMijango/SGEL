#pragma once
#include <string>

#include "Sprite.hpp"

/**
 * @class Animation
 * @brief Manages sprite animation frame timing and sequencing.
 *
 * Handles frame-by-frame animation playback. Supports both static sprites (single frame)
 * and multi-frame animations. Updates automatically each frame and loops upon completion.
 *
 * @see Sprite, Assets
 */
class Animation {
    /// Total number of animation frames
    Uint32          m_frameCount = 1;
    /// Current frame counter (incremented per update)
    Uint32          m_currentFrame = 0;
    /// Speed factor: frames to display per animation frame (higher = slower)
    Uint32          m_speed = 0;
    /// Unique name/identifier for the animation
    std::string     m_name = "none";
    /// Pointer to the SDL texture containing animation frames
    SDL_Texture*    m_texture = nullptr;
    /// Rectangle containing all animation frames in the texture
    SDL_FRect       m_animationRect = { 0, 0, 0, 0 };
    /// Current sprite rectangle to display
    SDL_FRect       m_spriteRect = { 0, 0, 0, 0 };

public:
    /**
     * @brief Constructs a static (single-frame) animation.
     * @param name Identifier for the animation.
     * @param texture Pointer to SDL_Texture.
     * @param spriteRect Rectangle defining the single sprite.
     */
    Animation(const std::string& name, SDL_Texture* texture, SDL_FRect spriteRect):
    Animation(name, texture, spriteRect, 1, 0)
    {
        m_spriteRect = spriteRect;
    }

    /**
     * @brief Constructs a multi-frame animation.
     * @param name Identifier for the animation.
     * @param texture Pointer to SDL_Texture containing all frames.
     * @param animationRect Rectangle containing all animation frames.
     * @param frameCount Number of frames in the animation.
     * @param speed Display speed: frames per animation frame (0 = instant, higher = slower).
     */
    Animation(const std::string& name, SDL_Texture* texture, SDL_FRect animationRect, Uint32 frameCount, Uint32 speed):
    m_frameCount(frameCount),
    m_currentFrame(0),
    m_speed(speed),
    m_name(name),
    m_texture(texture),
    m_animationRect(animationRect)
    {
        m_spriteRect = { animationRect.x, animationRect.y, animationRect.w / static_cast<float>(frameCount), animationRect.h };
    }

    /**
     * @brief Updates animation frame based on speed counter.
     *
     * Increments the frame counter and updates the sprite rectangle.
     * Automatically loops the animation upon reaching the final frame.
     * Should be called once per engine update.
     */
    void update() {
        m_currentFrame++;

        Uint32 spriteIndex = m_currentFrame / m_speed;

        if (spriteIndex >= m_frameCount) {
            // Animation completed one cycle, reset
            m_currentFrame = 0;
            m_spriteRect.x = m_animationRect.x;
        } else {
            m_spriteRect.x = spriteIndex * m_spriteRect.w + m_animationRect.x;
        }
    }

    /**
     * @brief Checks if the animation cycle has completed.
     * @return true if current frame exceeds frame count, false otherwise.
     */
    bool hasEnded() const {
        return m_currentFrame >= m_frameCount;
    }

    /**
     * @brief Gets the animation name.
     * @return The animation identifier.
     */
    std::string getName() const {
        return m_name;
    }

    /**
     * @brief Gets the current sprite for rendering.
     * @return A Sprite object with the current frame.
     */
    Sprite getSprite() const {
        return Sprite(m_texture, m_spriteRect);
    }

    /**
     * @brief Gets the underlying texture.
     * @return Pointer to SDL_Texture.
     */
    SDL_Texture* getTexture() const {
        return m_texture;
    }
};
