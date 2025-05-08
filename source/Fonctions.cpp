#include <cstdlib>
#include <ctime>
#include <ostream>
#include <iostream>
#include <sstream>
#include <algorithm> // Required for std::remove_if and std::erase
#include <vector>    // Required for std::vector

#include "Fonctions.hpp"
#include "Config.hpp"
#include "Player.hpp" // Make sure these are included if they define methods used here
#include "Enemy.hpp"
#include "Bullet.hpp"
#include "Background.hpp" // Assuming Background is used



bool initSDL(SDL_Window *&window, SDL_Renderer *&renderer, TTF_Font*& font, int width, int height)
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

    //initialize font
    font = TTF_OpenFont("text/Monas-BLBW8.ttf", 40);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

void closeSDL(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // TTF_CloseFont(font); // You might want to close the font here
    TTF_Quit(); // Quit SDL_ttf
    IMG_Quit(); // Quit SDL_image
    SDL_Quit();
}


void handleInput(SDL_Event &event, GameState &state)
{
    // Reset spacePressed state at the beginning of each frame's input handling
    // This assumes you only want to register a shot on a new key press
    // If you want continuous fire while space is held, handle that in handlePlayerShooting
    // For now, let's keep the spacePressed flag for single shots on press.
    // state.spacePressed = false; // Commenting this out based on your original code structure that sets it true on KEYDOWN

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        {
            state.running = false;
        }

        // Detect spacebar state
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                state.spacePressed = true;
            }
        } else if (event.type == SDL_KEYUP) {
             if (event.key.keysym.sym == SDLK_SPACE) {
                state.spacePressed = false;
            }
        }


        state.player.handleInput(event);
    }
}


void renderGame(SDL_Renderer *renderer, const GameState &state , TTF_Font* font )
{
    SDL_RenderClear(renderer);

    // Render background
    state.background.render();

    // Render player
    state.player.render();

    //render text
    renderHealthText(renderer, state.player, font);
    renderScoreText(renderer, state.player, font);

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


void checkGameOver(const Player& player, bool& running) {
    if (player.getHealth() <= 0) { // Check if health is 0 or less
        std::cout << "Game Over! Player health is below 0. Score: " << player.getScore() << std::endl;
        running = false;
    }
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

void renderHealthText(SDL_Renderer* renderer, const Player& player, TTF_Font* font) {
    SDL_Color redColor = {255, 0, 0, 255}; // Red for health
    std::stringstream healthText;
    healthText << "Health: " << player.getHealth();
    renderText(renderer, healthText.str().c_str(), redColor, 10, 10, font); // Top-left
}

void renderScoreText(SDL_Renderer* renderer, const Player& player, TTF_Font* font) {
    SDL_Color greenColor = {0, 255, 0, 255}; // Green for score
    std::stringstream scoreText;
    scoreText << "Score: " << player.getScore();

    int scoreTextWidth, scoreTextHeight;

    // Get text width and height without rendering
    TTF_SizeText(font, scoreText.str().c_str(), &scoreTextWidth, &scoreTextHeight);

    // Render score at the correct position (top-right)
    renderText(renderer, scoreText.str().c_str(), greenColor, Config::SCREEN_WIDTH - scoreTextWidth - 10, 10, font);
}


void handleWaveManagement(GameState &state)
{
    // Check if enemies are defeated and boss is not spawned
    if (!state.bossSpawned && state.enemies.empty())
    {
        // If there are more waves
        if (state.currentWave < 3) // Assuming 3 waves before boss based on your Config
        {
            int waveSize = Config::WAVE_MAX_ENEMIES[state.currentWave];
            for (int i = 0; i < waveSize; i++)
            {
                state.enemies.emplace_back(state.renderer); // Use state.renderer to pass to Enemy constructor
            }
            state.currentWave++;
            std::cout << "Starting Wave " << state.currentWave << std::endl;
        }
        // If all waves are cleared and boss is not spawned
        else if (state.currentWave == 3 && !state.bossSpawned)
        {
            state.boss = new Enemy(state.renderer, true); // Pass renderer to boss constructor
            state.bossSpawned = true;
            std::cout << "Boss Incoming!" << std::endl;
        }
         else {
             // All waves and boss defeated (or no more waves defined)
             // This is where you might handle winning the game
             std::cout << "You defeated all waves and the boss! You win!" << std::endl;
             state.running = false; // Example: End the game
         }
    }
}


void updateEnemiesAndBoss(GameState &state)
{
    // Update enemies
    for (auto &enemy : state.enemies)
    {
        enemy.update();
    }

    // Remove dead enemies (moved from updateBullets for better structure)
     state.enemies.erase(
        std::remove_if(
            state.enemies.begin(),
            state.enemies.end(),
            [](const Enemy &enemy)
            {
                return !enemy.isAlive(); // Assuming Enemy has isAlive() method
            }),
        state.enemies.end());


    // Update boss
    if (state.boss)
    {
        state.boss->update();
        // Check if boss is dead
        if (!state.boss->isAlive())
        {
            std::cout << "Boss defeated!" << std::endl;
            delete state.boss; // Clean up boss memory
            state.boss = nullptr; // Set boss pointer to null
            // Handle boss defeated logic (e.g., end game, next level)
            // state.running = false; // Example: End game after boss defeat
        }
    }
}


// Function to check for AABB collision
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b)
{
    // The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    // Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    // If any of the sides from A are outside of B
    if (bottomA <= topB) { return false; }
    if (topA >= bottomB) { return false; }
    if (rightA <= leftB) { return false; }
    if (leftA >= rightB) { return false; }

    // If none of the sides from A are outside B
    return true;

    // A simpler way using SDL_HasIntersection (requires SDL_rect.h, usually included with SDL.h)
    // return SDL_HasIntersection(&a, &b);
}



//----------------------------------------------------------------- shooting

bool loadBulletTextures(SDL_Renderer *renderer, GameState &state)
{
    // Load bullet textures
    state.playerBulletTexture = IMG_LoadTexture(renderer, Config::PLAYER_BULLET_TEXTURE.c_str());
    state.enemyBulletTexture = IMG_LoadTexture(renderer, Config::ENEMY_BULLET_TEXTURE.c_str());

    // remmove - keep these print statements for debugging if needed
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

    // Check if space is pressed AND enough time has passed since the last shot
    if (state.spacePressed && state.playerFireTimer >= 1.0f / Config::PLAYER_FIRE_RATE)
    {
        state.playerFireTimer = 0.0f; // Reset the timer

        // Calculate bullet spawn position relative to the player
        SDL_Rect bulletPosition = {
            state.player.getPosition().x + state.player.getPosition().w / 2 - Config::PLAYER_BULLET_WIDTH / 2,
            state.player.getPosition().y + state.player.getPosition().h / 2 - Config::PLAYER_BULLET_HEIGHT / 2, // Spawn bullet vertically centered
            Config::PLAYER_BULLET_WIDTH,
            Config::PLAYER_BULLET_HEIGHT
            };

        // Create and add the player bullet
        state.playerBullets.emplace_back(
            bulletPosition,
            Config::PLAYER_BULLET_SPEED, // Positive X velocity for horizontal movement
            0,                           // No vertical movement
            Config::PLAYER_BULLET_DAMAGE,
            state.playerBulletTexture);
    }
}

void handleEnemyShooting(GameState &state, float deltaTime)
{
    state.enemyFireTimer += deltaTime;

    // Check if enough time has passed since the last enemy shot
    if (state.enemyFireTimer >= 1.0f / Config::ENEMY_FIRE_RATE)
    {
        state.enemyFireTimer = 0.0f; // Reset the timer

        // Find a random enemy to shoot from
        if (!state.enemies.empty())
        {
            int randomIndex = rand() % state.enemies.size();
            const auto &enemy = state.enemies[randomIndex];

            // Calculate bullet spawn position relative to the enemy
            SDL_Rect bulletPosition = {
                enemy.getPosition().x + enemy.getPosition().w / 2 - Config::ENEMY_BULLET_WIDTH / 2,
                enemy.getPosition().y + enemy.getPosition().h / 2 - Config::ENEMY_BULLET_HEIGHT / 2, // Spawn bullet vertically centered
                Config::ENEMY_BULLET_WIDTH,
                Config::ENEMY_BULLET_HEIGHT
                };

            // Create and add the enemy bullet
            state.enemyBullets.emplace_back(
                bulletPosition,
                Config::ENEMY_BULLET_SPEED, // Negative X velocity for horizontal movement
                0,                          // No vertical movement
                Config::ENEMY_BULLET_DAMAGE,
                state.enemyBulletTexture);
        }
        // Handle boss shooting
        else if (state.boss)
        {
             SDL_Rect bulletPosition = {
                state.boss->getPosition().x + state.boss->getPosition().w / 2 - Config::ENEMY_BULLET_WIDTH / 2,
                state.boss->getPosition().y + state.boss->getPosition().h / 2 - Config::ENEMY_BULLET_HEIGHT / 2, // Spawn bullet vertically centered
                Config::ENEMY_BULLET_WIDTH,
                Config::ENEMY_BULLET_HEIGHT
                };

            state.enemyBullets.emplace_back(
                bulletPosition,
                Config::ENEMY_BULLET_SPEED, // Negative X velocity
                0,                          // No vertical movement
                Config::ENEMY_BULLET_DAMAGE * 2, // Maybe boss bullets do more damage? Example.
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

    // Check for player bullet collisions with enemies
    // We use iterators and erase to safely remove bullets/enemies during iteration
    for (auto itBullet = state.playerBullets.begin(); itBullet != state.playerBullets.end(); )
    {
        bool hitEnemy = false;
        for (auto itEnemy = state.enemies.begin(); itEnemy != state.enemies.end(); ++itEnemy)
        {
            // Use getCollisionBox for enemy
            if (checkCollision(itBullet->getPosition(), itEnemy->getCollisionBox()))
            {
                // Handle collision: reduce enemy health, increase player score, etc.
                itEnemy->decreaseHealth(itBullet->getDamage());
                state.player.increaseScore(10); // Increase score on hit

                hitEnemy = true;
                break; // Bullet hit one enemy, no need to check others
            }
        }

        // Check for player bullet collisions with boss
        if (state.boss && !hitEnemy) // Only check boss if not already removed by an enemy collision this frame
        {
             // Use getCollisionBox for boss
             if (checkCollision(itBullet->getPosition(), state.boss->getCollisionBox()))
             {
                 state.boss->decreaseHealth(itBullet->getDamage());
                 state.player.increaseScore(20); // Increase score more for hitting boss

                 hitEnemy = true; // Bullet hit the boss
             }
        }


        // Remove bullet if it hit something or is off-screen
        if (hitEnemy || itBullet->isOffScreen(screenWidth, screenHeight))
        {
            itBullet = state.playerBullets.erase(itBullet); // Remove the bullet and get the iterator to the next element
        }
        else
        {
            ++itBullet; // Move to the next bullet
        }
    }

    // Update enemy bullets
    for (auto &bullet : state.enemyBullets)
    {
        bullet.update();
    }

    // Check for enemy bullet collisions with player
    SDL_Rect playerCollisionBox = state.player.getCollisionBox(); // Get player's collision box once per update
    for (auto itBullet = state.enemyBullets.begin(); itBullet != state.enemyBullets.end(); )
    {
        // Use playerCollisionBox for player
        if (checkCollision(itBullet->getPosition(), playerCollisionBox))
        {
            // Handle collision: reduce player health
            state.player.decreaseHealth(itBullet->getDamage());

            itBullet = state.enemyBullets.erase(itBullet); // Remove the enemy bullet
        }
        else
        {
            ++itBullet; // Move to the next bullet
        }
    }

    // Note: Removing dead enemies and boss is handled in updateEnemiesAndBoss,
    // not here in updateBullets.
}


// Function to check for collisions between player and enemies/boss
void checkPlayerEnemyCollisions(GameState& state)
{
    SDL_Rect playerCollisionBox = state.player.getCollisionBox(); // Get player's collision box

    // Check collisions with regular enemies
    for (auto itEnemy = state.enemies.begin(); itEnemy != state.enemies.end(); )
    {
        // Use getCollisionBox for both player and enemy
        if (checkCollision(playerCollisionBox, itEnemy->getCollisionBox()))
        {
            // Handle collision: e.g., reduce player health, remove enemy
            state.player.decreaseHealth(20); // Example damage for player-enemy collision
            itEnemy = state.enemies.erase(itEnemy); // Remove the enemy on collision
        }
        else
        {
            ++itEnemy; // Move to the next enemy
        }
    }

    // Check collision with boss if spawned
    if (state.boss)
    {
         // Use getCollisionBox for both player and boss
         if (checkCollision(playerCollisionBox, state.boss->getCollisionBox()))
         {
            // Handle collision with boss
            state.player.decreaseHealth(50); // Example damage for player-boss collision
            // You might not want to remove the boss on collision, maybe just deal damage to the player
            // If the boss should be removed on collision, add: delete state.boss; state.boss = nullptr;
            // But be careful about how boss defeat is handled (usually by reducing health).
         }
    }
}

