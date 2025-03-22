#pragma once
#include <SDL2/SDL.h>

class Bullet
{
public:
    // Constructor
    Bullet(SDL_Rect position, int velocityX, int velocityY, int damage, SDL_Texture *texture);

    // Update bullet position
    void update();

    // Check if bullet is off-screen
    bool isOffScreen(int screenWidth, int screenHeight) const;

    // Getters
    SDL_Rect getPosition() const;
    int getDamage() const;
    void render(SDL_Renderer *renderer) const;
    SDL_Renderer *getRenderer() const;

private:
    SDL_Rect position; // Current position
    int velocityX;     // Horizontal speed and direction
    int velocityY;     // Vertical speed and direction
    int damage;        // Damage value
    SDL_Renderer *renderer;
    SDL_Texture *texture;
};
