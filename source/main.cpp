#include "Fonctions.hpp"

int main()
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    // Initializing
    srand(time(NULL));
    float deltaTime = 0.016f; // Approximate time per frame (60 FPS)

    if (!initSDL(window, renderer, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT))
    {
        return 1;
    }

    // Initialize GameState with all members
    GameState state{
        renderer,             // renderer
        Player(renderer),     // player
        Background(renderer), // background
        {},                   // enemies (empty vector)
        nullptr,              // boss (nullptr initially)
        0,                    // currentWave
        false,                // bossSpawned
        true,                 // running
        {},                   // playerBullets (empty vector)
        {},                   // enemyBullets (empty vector)
        0.0f,                 // playerFireTimer (initialized to 0.0f)
        0.0f                  // enemyFireTimer (initialized to 0.0f)
    };

    // bullet hell
    if (!loadBulletTextures(renderer, state))
    {
        return 1;
    }

    // Main game loop
    while (state.running)
    {
        SDL_Event event;
        handleInput(event, state);
        handleWaveManagement(state);

        // Update shooting and bullets
        handlePlayerShooting(state, deltaTime);
        handleEnemyShooting(state, deltaTime);
        updateBullets(state, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT);

        // Update core game elements
        state.player.update();
        state.background.update();
        updateEnemiesAndBoss(state);

        // Render everything
        renderGame(renderer, state);

        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    delete state.boss;
    SDL_DestroyTexture(state.playerBulletTexture);
    SDL_DestroyTexture(state.enemyBulletTexture);

    closeSDL(window, renderer);
    return 0;
}
