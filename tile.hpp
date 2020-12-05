#ifndef TILE_HPP_INCLUDED
#define TILE_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Tile
{
    //288
    sf::Sprite sprite;
    //8
    unsigned long width;
    unsigned long height;

public:
    Tile();
    Tile(const sf::Texture& texture);
    Tile(const sf::Texture& texture, unsigned long width, unsigned long height);
    void setPosition(unsigned long x, unsigned long y);

    void draw(sf::RenderWindow& window);
};

#endif // TILE_HPP_INCLUDED
