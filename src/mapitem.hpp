#ifndef MAPITEM_HPP_INCLUDED
#define MAPITEM_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class MapItem
{
    //288
    sf::Sprite sprite;

public:
    void setTexture(const sf::Texture& texture);
    void setPosition(long x, long y);

    bool contains(sf::Vector2f coords);
    void draw(sf::RenderWindow& window);
};

#endif // MAPITEM_HPP_INCLUDED
