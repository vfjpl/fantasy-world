#include "mapitem.hpp"

void MapItem::setTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture, true);
}

void MapItem::setPosition(long x, long y)
{
    sprite.setPosition((x * 32) - 32,
                       (y * 32) - 32);
}

bool MapItem::contains(sf::Vector2f coords)
{
    return sprite.getGlobalBounds().contains(coords);
}

void MapItem::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
