#ifndef TILE_HPP_INCLUDED
#define TILE_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Tile
{
    //288
    sf::Sprite sprite;
    //8
    long position_x;
    long position_y;
    long rect_width;
    long rect_height;

public:
    Tile();
    Tile(const sf::Texture& texture);
    Tile(const sf::Texture& texture, long width, long height);
    void setTexture(const sf::Texture& texture);
    void setPosition(long x, long y);

    bool isOnPosition(long x, long y);
    bool contains(sf::Vector2f coords);
    void draw(sf::RenderWindow& window);
};

#endif // TILE_HPP_INCLUDED
