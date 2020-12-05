#include "item.hpp"

void Item::setTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture);
}

void Item::setPosition(unsigned long x, unsigned long y)
{
    sprite.setPosition((32 * x) - 32, (32 * y) - 32);
}

bool Item::contains(sf::Vector2f coords)
{
    return sprite.getGlobalBounds().contains(coords);
}

void Item::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
