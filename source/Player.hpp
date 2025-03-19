#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Config.hpp"

class Player {
public:
    Player(SDL_Renderer* renderer);
    ~Player();
    void handleInput(const SDL_Event& event);
    void update();
    void render();
    
    SDL_Rect position;
    int health = 100;
    int score = 0;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int velocityX = 0;
    int velocityY = 0;
    void keepInBounds();
};

