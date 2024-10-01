#ifndef CHEST_HPP_INCLUDED
#define CHEST_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Chest
{
    //288
    sf::Sprite sprite;

public:
    void setTexture(const sf::Texture& texture);
    void setPosition(long x, long y);

    bool contains(sf::Vector2f coords);
    void draw(sf::RenderWindow& window);
};

#endif // CHEST_HPP_INCLUDED
