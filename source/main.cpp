#include "Fonctions.hpp"

int main()
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font* font = nullptr;

    // Initializing
    srand(time(NULL));
    float deltaTime = 0.016f; // Approximate time per frame (60 FPS)

    if (!initSDL(window, renderer, font, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT))
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

    // Load textures for bullets
    if (!loadBulletTextures(renderer, state))
    {
        // Handle error if textures fail to load
        // The initSDL function already checks for some errors,
        // you might want more specific error handling here or within loadBulletTextures
        return 1;
    }


    // Main game loop
    while (state.running)
    {
        SDL_Event event;
        handleInput(event, state); // Handle player input

        handleWaveManagement(state); // Manage enemy waves and boss spawning

        // Update shooting and bullets
        handlePlayerShooting(state, deltaTime); // Process player shooting
        handleEnemyShooting(state, deltaTime); // Process enemy shooting
        updateBullets(state, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT); // Update bullet positions and handle bullet collisions

        // Update core game elements
        state.player.update(); // Update player position
        state.background.update(); // Update background position
        updateEnemiesAndBoss(state); // Update enemy/boss positions and remove dead ones

        // Check for player-enemy/boss collisions
        // This should happen after objects have moved and bullets have hit
        checkPlayerEnemyCollisions(state); // Call the new collision function

        // Check game over condition
        // Check after all potential damage from collisions has been applied
        checkGameOver(state.player, state.running); // Check if player health is <= 0

        // Render everything
        renderGame(renderer, state , font); // Draw all game objects

        SDL_Delay(16); // Cap frame rate (~60 FPS)
    }

    // Cleanup - Free resources
    delete state.boss; // Delete boss if it was created
    state.boss = nullptr; // Set pointer to null
    SDL_DestroyTexture(state.playerBulletTexture); // Free bullet textures
    state.playerBulletTexture = nullptr; // Set pointer to null
    SDL_DestroyTexture(state.enemyBulletTexture); // Free bullet textures
    state.enemyBulletTexture = nullptr; // Set pointer to null


    closeSDL(window, renderer); // Clean up SDL
    return 0;
}
