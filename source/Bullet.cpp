#include "Bullet.hpp"

Bullet::Bullet(SDL_Rect position, int velocityX, int velocityY, int damage, SDL_Texture *texture)
    : position(position), velocityX(velocityX), velocityY(velocityY),
      damage(damage), texture(texture) {}

void Bullet::render(SDL_Renderer *renderer) const
{
    SDL_RenderCopy(renderer, texture, NULL, &position);
}

void Bullet::update()
{
    position.x += velocityX;
    position.y += velocityY;
}

bool Bullet::isOffScreen(int screenWidth, int screenHeight) const
{
    return position.x < 0 || position.x > screenWidth ||
           position.y < 0 || position.y > screenHeight;
}

SDL_Rect Bullet::getPosition() const
{
    return position;
}

int Bullet::getDamage() const
{
    return damage;
}

SDL_Renderer *Bullet::getRenderer() const { return renderer; }
