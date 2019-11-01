#include "monster.hpp"

void Monster::set_texture(const sf::Texture& texture, int width, int height)
{
    rect.width = width;
    rect.height = height;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void Monster::set_position(int x, int y)
{
    sprite.setPosition(32 * x, 32 * y);
}

void Monster::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
