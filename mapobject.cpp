#include "mapobject.hpp"

void MapObject::setTexture(const sf::Texture& texture, unsigned long width, unsigned long height)
{
    sprite.setTexture(texture, true);
    rect_width = width;
    rect_height = height;
}

void MapObject::setPosition(unsigned long x, unsigned long y)
{
    sprite.setPosition((32 * x) - 32 - ((rect_width - 32)/2),
                       (32 * y) - 32 - (rect_height - 32));
}

void MapObject::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
