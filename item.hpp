#ifndef ITEM_HPP_INCLUDED
#define ITEM_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Item
{
    //288
    sf::Sprite sprite;

public:
    void setTexture(const sf::Texture& texture);
    void set_position(unsigned long x, unsigned long y);

    bool contains(sf::Vector2f coords);
    void draw(sf::RenderWindow& window);
};

#endif // ITEM_HPP_INCLUDED
