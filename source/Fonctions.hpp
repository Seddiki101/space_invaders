#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

bool initSDL(SDL_Window*& window, SDL_Renderer*& renderer, int width, int height);
void closeSDL(SDL_Window* window, SDL_Renderer* renderer);
void renderText(SDL_Renderer* renderer, const char* text, SDL_Color color, int x, int y, TTF_Font* font);

