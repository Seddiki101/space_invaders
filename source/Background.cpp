#include "Background.hpp"
#include <iostream>

Background::Background(SDL_Renderer *renderer) : renderer(renderer)
{
    texture = IMG_LoadTexture(renderer, Config::BACKGROUND_TEXTURE.c_str());

    texture = IMG_LoadTexture(renderer, Config::BACKGROUND_TEXTURE.c_str());
    if (!texture)
    {
        std::cerr << "Failed to load background texture: " << IMG_GetError() << std::endl;
    }

    position1 = {0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT};
    position2 = {Config::SCREEN_WIDTH, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT};
}

Background::~Background()
{
    SDL_DestroyTexture(texture);
}

void Background::update()
{
    position1.x -= 1;
    position2.x -= 1;

    if (position1.x + Config::SCREEN_WIDTH <= 0)
        position1.x = Config::SCREEN_WIDTH;
    if (position2.x + Config::SCREEN_WIDTH <= 0)
        position2.x = Config::SCREEN_WIDTH;
}

void Background::render() const
{
    SDL_RenderCopy(renderer, texture, NULL, &position1);
    SDL_RenderCopy(renderer, texture, NULL, &position2);
}
