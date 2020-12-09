#ifndef TILE_HPP_INCLUDED
#define TILE_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Tile
{
    //288
    sf::Sprite sprite;
    //8
    unsigned long rect_width;
    unsigned long rect_height;

public:
    Tile(const sf::Texture& texture, unsigned long width = 32, unsigned long height = 32);
    void setPosition(unsigned long x, unsigned long y);

    bool contains(sf::Vector2f coords);
    void draw(sf::RenderWindow& window);
};

#endif // TILE_HPP_INCLUDED
