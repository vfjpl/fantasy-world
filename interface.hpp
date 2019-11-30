#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Interface
{
    //24
    std::vector<sf::Sprite> elements;

public:
    void setup(sf::Vector2u size);
    void draw(sf::RenderWindow& window);
};

#endif // INTERFACE_HPP_INCLUDED
