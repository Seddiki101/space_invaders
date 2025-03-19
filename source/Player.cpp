#include "Player.hpp"
#include <iostream>

Player::Player(SDL_Renderer* renderer) : renderer(renderer) {
    texture = IMG_LoadTexture(renderer, Config::PLAYER_TEXTURE.c_str());
    
        if (!texture) {
        std::cerr << "Failed to load player texture: " << IMG_GetError() << std::endl;
    }
    
    position = {
        Config::SCREEN_WIDTH/2 - Config::PLAYER_WIDTH/2,
        Config::SCREEN_HEIGHT/2 - Config::PLAYER_HEIGHT/2,
        Config::PLAYER_WIDTH,
        Config::PLAYER_HEIGHT
    };
}

Player::~Player() {
    SDL_DestroyTexture(texture);
}

void Player::handleInput(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        bool isKeyDown = (event.type == SDL_KEYDOWN);
        switch(event.key.keysym.sym) {
            case SDLK_UP: velocityY = isKeyDown ? -Config::PLAYER_SPEED : 0; break;
            case SDLK_DOWN: velocityY = isKeyDown ? Config::PLAYER_SPEED : 0; break;
            case SDLK_LEFT: velocityX = isKeyDown ? -Config::PLAYER_SPEED : 0; break;
            case SDLK_RIGHT: velocityX = isKeyDown ? Config::PLAYER_SPEED : 0; break;
        }
    }
}

void Player::update() {
    position.x += velocityX;
    position.y += velocityY;
    keepInBounds();
}

void Player::render() {
    SDL_RenderCopy(renderer, texture, NULL, &position);
}

void Player::keepInBounds() {
    if (position.x < 0) position.x = 0;
    if (position.y < 0) position.y = 0;
    if (position.x > Config::SCREEN_WIDTH - position.w) position.x = Config::SCREEN_WIDTH - position.w;
    if (position.y > Config::SCREEN_HEIGHT - position.h) position.y = Config::SCREEN_HEIGHT - position.h;
}
