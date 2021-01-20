#include "mapobject.hpp"

void MapObject::setTexture(const sf::Texture& texture, unsigned long width, unsigned long height)
{
    rect_width = width;
    rect_height = height;
    sprite.setTexture(texture, true);
}

void MapObject::setPosition(unsigned long x, unsigned long y)
{
    sprite.setPosition((x * 32) - 32 - ((rect_width - 32)/2),
                       (y * 32) - 32 - (rect_height - 32));
}

void MapObject::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
