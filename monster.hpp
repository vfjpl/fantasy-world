#ifndef MONSTER_HPP_INCLUDED
#define MONSTER_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Monster
{
    sf::Sprite sprite;
    sf::IntRect rect;

public:
    void set_texture(const sf::Texture& texture, int width, int height);
    void set_position(int x, int y);
    void draw(sf::RenderWindow& window);
};

#endif // MONSTER_HPP_INCLUDED
