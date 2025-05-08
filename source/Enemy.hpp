#pragma once
#include <SDL2/SDL.h>
#include "Config.hpp"

class Enemy
{
public:
    Enemy(SDL_Renderer *renderer, bool isBoss = false);
    void update();
    void render() const;
    bool isAlive() const;
    
   void decreaseHealth(int amount);

    SDL_Rect getPosition() const;
    SDL_Rect getCollisionBox() const;

private:
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    float speed;
    float velocityX;
    float velocityY;
    void randomizeMovement();
    void keepInBounds();
    SDL_Rect position;
    int health;
};
