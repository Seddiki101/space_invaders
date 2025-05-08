#include "Enemy.hpp"
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

Enemy::Enemy(SDL_Renderer *renderer, bool isBoss) : renderer(renderer)
{
    if (isBoss)
    {
        texture = IMG_LoadTexture(renderer, Config::BOSS_TEXTURE.c_str());
        health = Config::BOSS_HEALTH;
        speed = Config::BOSS_SPEED;
    }
    else
    {
        int randomIndex = rand() % 3;
        texture = IMG_LoadTexture(renderer, Config::ENEMY_TEXTURES[randomIndex].c_str());
        health = Config::ENEMY_HEALTH;
        speed = Config::ENEMY_SPEED;
    }

    position = {
        Config::SCREEN_WIDTH - Config::ENEMY_WIDTH,
        Config::SPAWN_AREA_MIN_Y + (rand() % (Config::SPAWN_AREA_MAX_Y - Config::SPAWN_AREA_MIN_Y)),
        Config::ENEMY_WIDTH,
        Config::ENEMY_HEIGHT};

    randomizeMovement();
}

void Enemy::update()
{
    position.x += velocityX;
    position.y += velocityY;
    keepInBounds();

    // Randomly change direction
    if (rand() % 100 < 2)
    { // 2% chance each frame
        randomizeMovement();
    }
}

void Enemy::render() const
{
    SDL_RenderCopy(renderer, texture, NULL, &position);
}

bool Enemy::isAlive() const
{
    return health > 0;
}

void Enemy::randomizeMovement()
{
    velocityX = (rand() % 100 - 50) / 50.0f * speed;
    velocityY = (rand() % 100 - 50) / 50.0f * speed;
}

void Enemy::keepInBounds()
{
    // Vertical bounds
    if (position.y < Config::SPAWN_AREA_MIN_Y)
        position.y = Config::SPAWN_AREA_MIN_Y;
    if (position.y > Config::SPAWN_AREA_MAX_Y - position.h)
        position.y = Config::SPAWN_AREA_MAX_Y - position.h;

    // Horizontal bounds (right side only)
    if (position.x < Config::ENEMY_MOVE_BOUNDARY)
        position.x = Config::ENEMY_MOVE_BOUNDARY;
    if (position.x > Config::SCREEN_WIDTH - position.w)
        position.x = Config::SCREEN_WIDTH - position.w;
}

SDL_Rect Enemy::getPosition() const
{
    return position;
}



void Enemy::decreaseHealth(int amount)
{
    if (amount > 0)
    {
        health -= amount;
        if (health < 0) health = 0; // Ensure health doesn't go below zero
    }
    else
    {
        std::cerr << "Decrease amount must be positive!" << std::endl;
    }
}


SDL_Rect Enemy::getCollisionBox() const
{
    // For simplicity, let's use the same rectangle as the render position for enemies.
    return position;
}
