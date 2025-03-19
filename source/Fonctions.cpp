#include <cstdlib>
#include <ctime>
#include <ostream>
#include <iostream>


#include "Fonctions.hpp"
#include "Config.hpp"

bool initSDL(SDL_Window*& window, SDL_Renderer*& renderer, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_Error: " << TTF_GetError() << std::endl;
        return false;
    }
    
    
    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! Error: " << IMG_GetError() << std::endl;
        return false;
    }

    //window title aka game name
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void closeSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit(); // Quit SDL_ttf
    SDL_Quit();
}

void renderText(SDL_Renderer* renderer, const char* text, SDL_Color color, int x, int y, TTF_Font* font) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    
    if (!textTexture) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }
    
    SDL_Rect destRect = {x, y, 0, 0};
    SDL_QueryTexture(textTexture, NULL, NULL, &destRect.w, &destRect.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);
    
    // Free the texture after rendering
    SDL_DestroyTexture(textTexture);
}

void handleInput(SDL_Event& event, GameState& state) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || 
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            state.running = false;
        }
        state.player.handleInput(event);
    }
}

void handleWaveManagement(GameState& state) {
    if (!state.bossSpawned && state.enemies.empty()) {
        if (state.currentWave < 3) {
            int waveSize = Config::WAVE_MAX_ENEMIES[state.currentWave];
            for (int i = 0; i < waveSize; i++) {
                state.enemies.emplace_back(state.player.getRenderer() );
            }
            state.currentWave++;
        } else {
            state.boss = new Enemy(state.player.getRenderer(), true);
            state.bossSpawned = true;
        }
    }
}



void updateEnemiesAndBoss(GameState& state) {
    for (auto& enemy : state.enemies) {
        enemy.update();
    }
    
    if (state.boss) {
        state.boss->update();
    }
}


void renderGame(SDL_Renderer* renderer, const GameState& state) {
    SDL_RenderClear(renderer);
    
    // Render background
    state.background.render();
    
    // Render player
    state.player.render();
    
    // Render enemies
    for (const auto& enemy : state.enemies) {
        enemy.render();
    }
    
    // Render boss
    if (state.boss) {
        state.boss->render();
    }
    
    SDL_RenderPresent(renderer);
}
