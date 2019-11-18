#include "item.hpp"

void Item::setTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture);
}

void Item::set_position(int x, int y)
{
    sprite.setPosition((32 * x) - 32, (32 * y) - 32);
}

void Item::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
