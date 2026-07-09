#pragma once
#include <SDL3/SDL_render.h>

/**
 * @class Sprite
 * @brief Represents a drawable sprite with texture and destination rectangle.
 *
 * Simple POD-style class holding a reference to an SDL texture and the
 * rectangular region to render from that texture.
 *
 * @see Animation, Assets
 */
class Sprite {
public:
    /// Pointer to the SDL texture to render
    SDL_Texture*    m_texture;
    /// Rectangle defining the texture region to render
    SDL_FRect       m_textureRect;

    /**
     * @brief Constructs a sprite.
     * @param texture Pointer to SDL_Texture.
     * @param textureRect Rectangle within the texture to display.
     */
    Sprite(SDL_Texture* texture, const SDL_FRect& textureRect)
        : m_texture(texture), m_textureRect(textureRect) {}
};
