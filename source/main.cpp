#include "Fonctions.hpp"
#include "Player.hpp"
#include "Background.hpp"
#include "Config.hpp"
#include <ostream>
#include <iostream>

int main() {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    if (!initSDL(window, renderer, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT)) {
        return 1;
    }

    Player player(renderer);
    Background background(renderer);
    bool running = true;
    SDL_Event event;

    while (running) {
        // Process input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            player.handleInput(event);
        }

        // Update game state
        player.update();
        background.update();

        // Render
        SDL_RenderClear(renderer);
        background.render();
        player.render();
        SDL_RenderPresent(renderer);
        
     //   std::cout << "Game loop running..." << std::endl; // Debug print

        SDL_Delay(16); // ~60 FPS
    }

    closeSDL(window, renderer);
    return 0;
}
