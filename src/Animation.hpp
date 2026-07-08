#pragma once
#include <string>

#include "Sprite.hpp"

class Animation {
    Uint32          m_frameCount = 1;
    Uint32          m_currentFrame = 0;
    Uint32          m_speed = 0;
    std::string     m_name = "none";
    SDL_Texture*    m_texture = nullptr;
    SDL_FRect       m_animationRect = { 0, 0, 0, 0 };
    SDL_FRect       m_spriteRect = { 0, 0, 0, 0 };

public:
    Animation(const std::string& name, SDL_Texture* texture, SDL_FRect spriteRect):
    Animation(name, texture, spriteRect, 1, 0)
    {
        m_spriteRect = spriteRect;
    }

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

    void update() {
        m_currentFrame++;

        Uint32 spriteIndex = m_currentFrame / m_speed;

        if (spriteIndex >= m_frameCount) {
            // La animación completó un ciclo
            m_currentFrame = 0;
            m_spriteRect.x = m_animationRect.x;
        } else {
            m_spriteRect.x = spriteIndex * m_spriteRect.w + m_animationRect.x;
        }
    }

    bool hasEnded() const {
        return m_currentFrame >= m_frameCount;
    }

    std::string getName() const {
        return m_name;
    }

    Sprite getSprite() const {
        return Sprite(m_texture, m_spriteRect);
    }

    SDL_Texture* getTexture() const {
        return m_texture;
    }
};
