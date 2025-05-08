#include "Player.hpp"
#include <iostream>

Player::Player(SDL_Renderer *renderer) : renderer(renderer)
{
    texture = IMG_LoadTexture(renderer, Config::PLAYER_TEXTURE.c_str());

    if (!texture)
    {
        std::cerr << "Failed to load player texture: " << IMG_GetError() << std::endl;
    }

    position = {
        Config::SCREEN_WIDTH / 2 - Config::PLAYER_WIDTH / 2,
        Config::SCREEN_HEIGHT / 2 - Config::PLAYER_HEIGHT / 2,
        Config::PLAYER_WIDTH,
        Config::PLAYER_HEIGHT};
}

Player::~Player()
{
    SDL_DestroyTexture(texture);
}

void Player::handleInput(const SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
    {
        bool isKeyDown = (event.type == SDL_KEYDOWN);
        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
            velocityY = isKeyDown ? -Config::PLAYER_SPEED : 0;
            break;
        case SDLK_DOWN:
            velocityY = isKeyDown ? Config::PLAYER_SPEED : 0;
            break;
        case SDLK_LEFT:
            velocityX = isKeyDown ? -Config::PLAYER_SPEED : 0;
            break;
        case SDLK_RIGHT:
            velocityX = isKeyDown ? Config::PLAYER_SPEED : 0;
            break;
        }
    }
}

void Player::update()
{
    position.x += velocityX;
    position.y += velocityY;
    keepInBounds();
}

void Player::render() const
{
    SDL_RenderCopy(renderer, texture, NULL, &position);
}

void Player::keepInBounds()
{
    if (position.x < 0)
        position.x = 0;
    if (position.y < 0)
        position.y = 0;
    if (position.x > Config::SCREEN_WIDTH - position.w)
        position.x = Config::SCREEN_WIDTH - position.w;
    if (position.y > Config::SCREEN_HEIGHT - position.h)
        position.y = Config::SCREEN_HEIGHT - position.h;
}


SDL_Renderer *Player::getRenderer() const { return renderer; }

SDL_Rect Player::getPosition() const
{
    return position;
}




int Player::getHealth() const
{
    return health;
}

// Setter for health
void Player::setHealth(int newHealth)
{
    if (newHealth >= 0)
    { 
        health = newHealth;
    }
    else
    {
        std::cerr << "Health cannot be negative!" << std::endl;
    }
}

// Getter for score
int Player::getScore() const
{
    return score;
}

// Setter for score
void Player::setScore(int newScore)
{
    if (newScore >= 0)
    { 
        score = newScore;
    }
    else
    {
        std::cerr << "Score cannot be negative!" << std::endl;
    }
}

void Player::increaseHealth(int amount)
{
    if (amount > 0)
    {
        health += amount;
    }
    else
    {
        std::cerr << "Increase amount must be positive!" << std::endl;
    }
}

void Player::increaseScore(int amount)
{
    if (amount > 0)
    {
        score += amount;
    }
    else
    {
        std::cerr << "Increase amount must be positive!" << std::endl;
    }
}

void Player::decreaseHealth(int amount)
{
    if (amount > 0)
    {
        health -= amount;
         if (health < 0) health = 0;
    }
    else
    {
        std::cerr << "Decrease amount must be positive!" << std::endl;
    }
}

void Player::decreaseScore(int amount)
{
    if (amount > 0)
    {
        score -= amount;
    }
    else
    {
        std::cerr << "Decrease amount must be positive!" << std::endl;
    }
}


SDL_Rect Player::getCollisionBox() const
{
    // You can define a collision box different from the rendering position
    // based on your Config values if needed.
    // For example, a smaller box centered within the sprite:
    SDL_Rect collisionBox;
    collisionBox.w = Config::PLAYER_COLLISION_WIDTH;
    collisionBox.h = Config::PLAYER_COLLISION_HEIGHT;
    collisionBox.x = position.x + (position.w - collisionBox.w) / 2;
    collisionBox.y = position.y + (position.h - collisionBox.h) / 2;

    // If the collision box is the same as the position, just return position:
    // return position;

    return collisionBox;
}
