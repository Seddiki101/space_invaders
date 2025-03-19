#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Config.hpp"

class Background {
public:
    Background(SDL_Renderer* renderer);
    ~Background();
    void update();
    void render()const;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect position1;
    SDL_Rect position2;
};

