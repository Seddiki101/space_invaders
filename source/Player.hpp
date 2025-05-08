#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <mutex>
#include "Config.hpp"

class Player
{
public:
    Player(SDL_Renderer *renderer);
    ~Player();
    void handleInput(const SDL_Event &event);
    void update();
    void render() const;

    int getHealth() const;
    void setHealth(int newHealth);
    int getScore() const;
    void setScore(int newScore);

    void increaseHealth(int amount);
    void increaseScore(int amount);

    void decreaseHealth(int amount);
    void decreaseScore(int amount);

    SDL_Renderer *getRenderer() const;

    SDL_Rect getPosition() const;
    SDL_Rect getCollisionBox() const;

private:
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int velocityX = 0;
    int velocityY = 0;
    void keepInBounds();

    // this is useless because currently this is single threaded
    // std::mutex mtx;

    SDL_Rect position;
    int health = 100;
    int score = 0;
};
