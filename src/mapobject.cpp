#include "mapobject.hpp"

void MapObject::setTexture(const sf::Texture& texture, long width, long height)
{
    rect_width = width;
    rect_height = height;
    sprite.setTexture(texture, true);
}

void MapObject::setPosition(long x, long y)
{
    const int px_x = (x * 32l) - 32l - ((rect_width - 32l)/2l);
    const int px_y = (y * 32l) - 32l - (rect_height - 32l);
    sprite.setPosition(px_x, px_y);//float conversion
}

void MapObject::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
