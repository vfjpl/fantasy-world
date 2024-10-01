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


Tile::Tile(const sf::Texture& texture, long width, long height):
    sprite(texture),
    rect_width(width),
    rect_height(height) {}


void Tile::setTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture, true);
}

void Tile::setPosition(long x, long y)
{
    position_x = x;
    position_y = y;
    const int px_x = (x * 32) - 32 - ((rect_width - 32)/2);
    const int px_y = (y * 32) - 32 - (rect_height - 32);
    sprite.setPosition(px_x, px_y);//float conversion
}

bool Tile::isOnPosition(long x, long y)
{
    return (position_x == x && position_y == y);
}

bool Tile::contains(sf::Vector2f coords)
{
    return sprite.getGlobalBounds().contains(coords);
}

void Tile::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
