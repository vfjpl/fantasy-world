#ifndef DOOR_HPP_INCLUDED
#define DOOR_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Door
{
    //288
    sf::Sprite sprite;

public:
    Door(const sf::Texture& texture);
    void set_position(unsigned long x, unsigned long y);

    void draw(sf::RenderWindow& window);
};

#endif // DOOR_HPP_INCLUDED
