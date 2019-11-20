#ifndef DOOR_HPP_INCLUDED
#define DOOR_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Door
{
    sf::Sprite sprite;

public:
    Door(const sf::Texture& texture);
    void setTexture(const sf::Texture& texture);
    void set_position(int x, int y);
    void draw(sf::RenderWindow& window);
};

#endif // DOOR_HPP_INCLUDED
