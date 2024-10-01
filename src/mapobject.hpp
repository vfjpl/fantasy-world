#ifndef MAPOBJECT_HPP_INCLUDED
#define MAPOBJECT_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class MapObject
{
    //288
    sf::Sprite sprite;
    //8
    long rect_width;
    long rect_height;

public:
    void setTexture(const sf::Texture& texture, long width, long height);
    void setPosition(long x, long y);

    void draw(sf::RenderWindow& window);
};

#endif // MAPOBJECT_HPP_INCLUDED
