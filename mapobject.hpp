#ifndef MAPOBJECT_HPP_INCLUDED
#define MAPOBJECT_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class MapObject
{
    //288
    sf::Sprite sprite;

public:
    void setTexture(const sf::Texture& texture, unsigned long width, unsigned long height);
    void setPosition(unsigned long x, unsigned long y);

    void draw(sf::RenderWindow& window);
};

#endif // MAPOBJECT_HPP_INCLUDED
