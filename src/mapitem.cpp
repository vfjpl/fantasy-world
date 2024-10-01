#include "mapitem.hpp"

void MapItem::setTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture, true);
}

void MapItem::setPosition(long x, long y)
{
    const int px_x = (x * 32) - 32;
    const int px_y = (y * 32) - 32;
    sprite.setPosition(px_x, px_y);//float conversion
}

bool MapItem::contains(sf::Vector2f coords)
{
    return sprite.getGlobalBounds().contains(coords);
}

void MapItem::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
