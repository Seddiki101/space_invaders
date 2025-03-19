#pragma once
#include <string>

struct Config {
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;
    
    static const int PLAYER_WIDTH;
    static const int PLAYER_HEIGHT;
    static const int PLAYER_COLLISION_WIDTH;
    static const int PLAYER_COLLISION_HEIGHT;
    static const int PLAYER_SPEED;
    
    static const std::string PLAYER_TEXTURE;
    static const std::string BACKGROUND_TEXTURE;
};
