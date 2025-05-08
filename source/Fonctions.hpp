#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Player.hpp"
#include "Background.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"

struct GameState
{
    SDL_Renderer *renderer;
    Player player;
    Background background;
    std::vector<Enemy> enemies;
    Enemy *boss = nullptr;
    int currentWave = 0;
    bool bossSpawned = false;
    bool running = true;

    std::vector<Bullet> playerBullets;
    std::vector<Bullet> enemyBullets;

    // Add timers for fire rate
    float playerFireTimer = 0.0f;
    float enemyFireTimer = 0.0f;

    SDL_Texture *playerBulletTexture;
    SDL_Texture *enemyBulletTexture;

    bool spacePressed = false;
};

struct GameObject
{
    SDL_Rect position;
    SDL_Rect collisionBox;
};

bool initSDL(SDL_Window *&window, SDL_Renderer *&renderer,  TTF_Font*& font , int width, int height);
void closeSDL(SDL_Window *window, SDL_Renderer *renderer);

// Text rendering helper functions
void renderText(SDL_Renderer *renderer, const char *text, SDL_Color color, int x, int y, TTF_Font *font);
void renderHealthText(SDL_Renderer* renderer, const Player& player, TTF_Font* font);
void renderScoreText(SDL_Renderer* renderer, const Player& player, TTF_Font* font);

bool loadBulletTextures(SDL_Renderer *renderer, GameState &state);

void handleInput(SDL_Event &event, GameState &state);
void renderGame(SDL_Renderer *renderer, const GameState &state ,  TTF_Font* font  );

void handleWaveManagement(GameState &state);
void updateEnemiesAndBoss(GameState &state);

void updateBullets(GameState &state, int screenWidth, int screenHeight);
void handleEnemyShooting(GameState &state, float deltaTime);
void handlePlayerShooting(GameState &state, float deltaTime);

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
void checkPlayerEnemyCollisions(GameState& state);

void checkGameOver(const Player& player, bool& running); 
