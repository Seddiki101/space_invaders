#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Player.hpp"    
#include "Background.hpp" 
#include "Enemy.hpp"      

struct GameState {
 SDL_Renderer* renderer;
    Player player;
    Background background;
    std::vector<Enemy> enemies;
    Enemy* boss = nullptr;
    int currentWave = 0;
    bool bossSpawned = false;
    bool running = true;
};

bool initSDL(SDL_Window*& window, SDL_Renderer*& renderer, int width, int height);
void closeSDL(SDL_Window* window, SDL_Renderer* renderer);
void renderText(SDL_Renderer* renderer, const char* text, SDL_Color color, int x, int y, TTF_Font* font);

void handleInput(SDL_Event& event, GameState& state);
void handleWaveManagement(GameState& state);
void updateEnemiesAndBoss(GameState& state);
void renderGame(SDL_Renderer* renderer, const GameState& state);

