#include <cstdlib>
#include <ctime>
#include <ostream>
#include <iostream>
#include <algorithm>

#include "Fonctions.hpp"
#include "Config.hpp"

bool initSDL(SDL_Window *&window, SDL_Renderer *&renderer, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        std::cerr << "SDL_ttf could not initialize! SDL_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cerr << "SDL_image could not initialize! Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // window title aka game name
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void closeSDL(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit(); // Quit SDL_ttf
    SDL_Quit();
}

void renderText(SDL_Renderer *renderer, const char *text, SDL_Color color, int x, int y, TTF_Font *font)
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    if (!textSurface)
    {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (!textTexture)
    {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect destRect = {x, y, 0, 0};
    SDL_QueryTexture(textTexture, NULL, NULL, &destRect.w, &destRect.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    // Free the texture after rendering
    SDL_DestroyTexture(textTexture);
}

void handleInput(SDL_Event &event, GameState &state)
{
    bool spacePressed = false;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        {
            state.running = false;
        }

        // Detect spacebar state
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
        {
            spacePressed = true;
        }

        state.player.handleInput(event);
    }

    // Store spacebar state in GameState
    state.spacePressed = spacePressed;
}

void handleWaveManagement(GameState &state)
{
    if (!state.bossSpawned && state.enemies.empty())
    {
        if (state.currentWave < 3)
        {
            int waveSize = Config::WAVE_MAX_ENEMIES[state.currentWave];
            for (int i = 0; i < waveSize; i++)
            {
                state.enemies.emplace_back(state.player.getRenderer());
            }
            state.currentWave++;
        }
        else
        {
            state.boss = new Enemy(state.player.getRenderer(), true);
            state.bossSpawned = true;
        }
    }
}

void updateEnemiesAndBoss(GameState &state)
{
    for (auto &enemy : state.enemies)
    {
        enemy.update();
    }

    if (state.boss)
    {
        state.boss->update();
    }
}

void renderGame(SDL_Renderer *renderer, const GameState &state)
{
    SDL_RenderClear(renderer);

    // Render background
    state.background.render();

    // Render player
    state.player.render();

    // Render enemies
    for (const auto &enemy : state.enemies)
    {
        enemy.render();
    }

    // Render boss
    if (state.boss)
    {
        state.boss->render();
    }

    // Render player bullets
    for (const auto &bullet : state.playerBullets)
    {
        bullet.render(renderer);
    }

    // Render enemy bullets
    for (const auto &bullet : state.enemyBullets)
    {
        bullet.render(renderer);
    }

    SDL_RenderPresent(renderer);
}

//----------------------------------------------------------------- shooting

bool loadBulletTextures(SDL_Renderer *renderer, GameState &state)
{
    // Load bullet textures
    state.playerBulletTexture = IMG_LoadTexture(renderer, Config::PLAYER_BULLET_TEXTURE.c_str());
    state.enemyBulletTexture = IMG_LoadTexture(renderer, Config::ENEMY_BULLET_TEXTURE.c_str());

    // remmove
    std::cout << "Player bullet texture loaded: " << (state.playerBulletTexture ? "Yes" : "No") << "\n";
    std::cout << "Enemy bullet texture loaded: " << (state.enemyBulletTexture ? "Yes" : "No") << "\n";
    // remmove

    if (!state.playerBulletTexture || !state.enemyBulletTexture)
    {
        std::cerr << "Failed to load bullet textures!\n";
        return false;
    }
    return true;
}

void handlePlayerShooting(GameState &state, float deltaTime)
{
    state.playerFireTimer += deltaTime;

    if (state.spacePressed && state.playerFireTimer >= 1.0f / Config::PLAYER_FIRE_RATE)
    {
        state.playerFireTimer = 0.0f;

        SDL_Rect bulletPosition = {
            state.player.getPosition().x + state.player.getPosition().w / 2 - Config::PLAYER_BULLET_WIDTH / 2,
            state.player.getPosition().y - Config::PLAYER_BULLET_HEIGHT,
            Config::PLAYER_BULLET_WIDTH,
            Config::PLAYER_BULLET_HEIGHT};

        state.playerBullets.emplace_back(
            bulletPosition,
            Config::PLAYER_BULLET_SPEED, // Positive X velocity
            0,                           // No vertical movement
            Config::PLAYER_BULLET_DAMAGE,
            state.playerBulletTexture);
    }
}

void handleEnemyShooting(GameState &state, float deltaTime)
{
    state.enemyFireTimer += deltaTime;

    if (state.enemyFireTimer >= 1.0f / Config::ENEMY_FIRE_RATE)
    {
        state.enemyFireTimer = 0.0f;

        if (!state.enemies.empty())
        {
            int randomIndex = rand() % state.enemies.size();
            const auto &enemy = state.enemies[randomIndex];

            SDL_Rect bulletPosition = {
                enemy.getPosition().x + enemy.getPosition().w / 2 - Config::ENEMY_BULLET_WIDTH / 2,
                enemy.getPosition().y + enemy.getPosition().h,
                Config::ENEMY_BULLET_WIDTH,
                Config::ENEMY_BULLET_HEIGHT};

            state.enemyBullets.emplace_back(
                bulletPosition,
                Config::ENEMY_BULLET_SPEED, // Negative X velocity
                0,                          // No vertical movement
                Config::ENEMY_BULLET_DAMAGE,
                state.enemyBulletTexture);
        }
    }
}

void updateBullets(GameState &state, int screenWidth, int screenHeight)
{
    // Update player bullets
    for (auto &bullet : state.playerBullets)
    {
        bullet.update();
    }

    // Remove off-screen player bullets
    state.playerBullets.erase(
        std::remove_if(
            state.playerBullets.begin(),
            state.playerBullets.end(),
            [screenWidth, screenHeight](const Bullet &bullet)
            {
                return bullet.isOffScreen(screenWidth, screenHeight);
            }),
        state.playerBullets.end());

    // Update enemy bullets
    for (auto &bullet : state.enemyBullets)
    {
        bullet.update();
    }

    // Remove off-screen enemy bullets
    state.enemyBullets.erase(
        std::remove_if(
            state.enemyBullets.begin(),
            state.enemyBullets.end(),
            [screenWidth, screenHeight](const Bullet &bullet)
            {
                return bullet.isOffScreen(screenWidth, screenHeight);
            }),
        state.enemyBullets.end());
}
