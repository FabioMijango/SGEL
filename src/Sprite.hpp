#pragma once
#include <SDL3/SDL_render.h>

class Sprite {
public:
    SDL_Texture*    m_texture;
    SDL_FRect       m_textureRect;

    Sprite(SDL_Texture* texture, const SDL_FRect& textureRect)
        : m_texture(texture), m_textureRect(textureRect) {}
};
