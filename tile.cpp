#include "tile.hpp"

Tile::Tile():
    width(32),
    height(32) {}


Tile::Tile(const sf::Texture& texture):
    sprite(texture),
    width(32),
    height(32) {}


Tile::Tile(const sf::Texture& texture, unsigned long width, unsigned long height):
    sprite(texture),
    width(width),
    height(height) {}


void Tile::setPosition(unsigned long x, unsigned long y)
{
    sprite.setPosition((32 * x) - ((width - 32)/2) - 32,
                       (32 * y) - (height - 32) - 32);
}

void Tile::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
