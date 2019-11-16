#ifndef ITEM_HPP_INCLUDED
#define ITEM_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Item
{
    sf::Sprite sprite;

public:
    void setTexture(const sf::Texture& texture);
    void set_position(int x, int y);
    void draw(sf::RenderWindow& window);
};

#endif // ITEM_HPP_INCLUDED
