#include "Fonctions.hpp"

int main() {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    // Initializing
    if (!initSDL(window, renderer, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT)) {
        return 1;
    }
    
    GameState state{
         renderer,
         Player(renderer),
         Background(renderer),
        {},    // enemies
        nullptr, // boss
        0,     // currentWave
        false, // bossSpawned
        true   // running
    };
    srand(time(NULL));

    // Main game loop
    while (state.running) {
            SDL_Event event; 
        handleInput(event, state);
        handleWaveManagement(state);
        
        updateEnemiesAndBoss(state);
        
        // Update core game elements
        state.player.update();
        state.background.update();
        
        // Render everything
        renderGame(renderer, state);
        
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    delete state.boss;

    closeSDL(window, renderer);
    return 0;
}
