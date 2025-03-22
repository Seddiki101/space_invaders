#pragma once
#include <string>

struct Config
{
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;

    static const int PLAYER_WIDTH;
    static const int PLAYER_HEIGHT;
    static const int PLAYER_COLLISION_WIDTH;
    static const int PLAYER_COLLISION_HEIGHT;
    static const int PLAYER_SPEED;

    static const std::string PLAYER_TEXTURE;
    static const std::string BACKGROUND_TEXTURE;

    static const std::string PLAYER_BULLET_TEXTURE;
    static const std::string ENEMY_BULLET_TEXTURE;

    static const int ENEMY_WIDTH;
    static const int ENEMY_HEIGHT;
    static const float ENEMY_SPEED;
    static const float BOSS_SPEED;
    static const int ENEMY_HEALTH;
    static const int BOSS_HEALTH;
    static const std::string ENEMY_TEXTURES[3];
    static const std::string BOSS_TEXTURE;
    static const int WAVE_MAX_ENEMIES[3];
    static const int SPAWN_AREA_MIN_Y;
    static const int SPAWN_AREA_MAX_Y;
    static const int ENEMY_MOVE_BOUNDARY;

    static const int PLAYER_BULLET_DAMAGE;
    static const int ENEMY_BULLET_DAMAGE;
    static const int PLAYER_BULLET_SPEED;
    static const int ENEMY_BULLET_SPEED;
    static const float PLAYER_FIRE_RATE;
    static const float ENEMY_FIRE_RATE;

    static const int PLAYER_BULLET_WIDTH;
    static const int PLAYER_BULLET_HEIGHT;
    static const int ENEMY_BULLET_WIDTH;
    static const int ENEMY_BULLET_HEIGHT;
};
