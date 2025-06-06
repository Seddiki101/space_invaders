#include "Config.hpp"

const int Config::SCREEN_WIDTH = 1600;
const int Config::SCREEN_HEIGHT = 900;
const int Config::PLAYER_WIDTH = 103;
const int Config::PLAYER_HEIGHT = 49;
const int Config::PLAYER_COLLISION_WIDTH = 100;
const int Config::PLAYER_COLLISION_HEIGHT = 50;
const int Config::PLAYER_SPEED = 5;
const std::string Config::PLAYER_TEXTURE = "./images/player.png";
const std::string Config::BACKGROUND_TEXTURE = "./images/background.jpg";

const int Config::ENEMY_WIDTH = 64;
const int Config::ENEMY_HEIGHT = 64;
const float Config::ENEMY_SPEED = 2.5f;
const float Config::BOSS_SPEED = 1.2f;
const int Config::ENEMY_HEALTH = 29;
const int Config::BOSS_HEALTH = 100;
const std::string Config::ENEMY_TEXTURES[3] = {
    "./images/e1.png",
    "./images/e2.png",
    "./images/e3.png"};

const std::string Config::BOSS_TEXTURE = "./images/boss.png";
const int Config::WAVE_MAX_ENEMIES[3] = {3, 6, 9};
const int Config::SPAWN_AREA_MIN_Y = 100;
const int Config::SPAWN_AREA_MAX_Y = 800;
const int Config::ENEMY_MOVE_BOUNDARY = Config::SCREEN_WIDTH * 2 / 5;

const float Config::PLAYER_FIRE_RATE = 0.2f; // 5 shots per second
const float Config::ENEMY_FIRE_RATE = 1.0f;  // 1 shot per second

const int Config::PLAYER_BULLET_SPEED = 10; // Negative for upward movement
const int Config::ENEMY_BULLET_SPEED = -5;  // Positive for downward movement

const int Config::PLAYER_BULLET_DAMAGE = 10;
const int Config::ENEMY_BULLET_DAMAGE = 8;

const std::string Config::PLAYER_BULLET_TEXTURE = "./images/playerBullet.png";
const std::string Config::ENEMY_BULLET_TEXTURE = "./images/enemyBullet1.png";

const int Config::PLAYER_BULLET_WIDTH = 63;
const int Config::PLAYER_BULLET_HEIGHT = 15;
const int Config::ENEMY_BULLET_WIDTH = 66;
const int Config::ENEMY_BULLET_HEIGHT = 13;
