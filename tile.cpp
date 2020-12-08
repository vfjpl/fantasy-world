#include "tile.hpp"

Tile::Tile():
    rect_width(32),
    rect_height(32)
{
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}


Tile::Tile(const sf::Texture& texture):
    sprite(texture),
    rect_width(32),
    rect_height(32) {}


Tile::Tile(const sf::Texture& texture, unsigned long width, unsigned long height):
    sprite(texture),
    rect_width(width),
    rect_height(height) {}


void Tile::setPosition(unsigned long x, unsigned long y)
{
    sprite.setPosition((32 * x) - 32 - ((rect_width - 32)/2),
                       (32 * y) - 32 - (rect_height - 32));
}

bool Tile::contains(sf::Vector2f coords)
{
    return sprite.getGlobalBounds().contains(coords);
}

void Tile::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
