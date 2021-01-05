#include "mapitem.hpp"

void MapItem::setTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture, true);
}

void MapItem::setPosition(unsigned long x, unsigned long y)
{
    sprite.setPosition((32 * x) - 32,
                       (32 * y) - 32);
}

bool MapItem::contains(sf::Vector2f coords)
{
    return sprite.getGlobalBounds().contains(coords);
}

void MapItem::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
