#include "mapobject.hpp"

void MapObject::setTexture(const sf::Texture& texture, long width, long height)
{
    rect_width = width;
    rect_height = height;
    sprite.setTexture(texture, true);
}

void MapObject::setPosition(long x, long y)
{
    const int px_x = (x * 32) - 32 - ((rect_width - 32)/2);
    const int px_y = (y * 32) - 32 - (rect_height - 32);
    sprite.setPosition(px_x, px_y);//float conversion
}

void MapObject::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
