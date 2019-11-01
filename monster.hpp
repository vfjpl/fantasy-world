#ifndef MONSTER_HPP_INCLUDED
#define MONSTER_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Monster
{
    sf::Sprite sprite;

public:
    Monster(const sf::Texture& texture);

    void draw(sf::RenderWindow& window);
};

#endif // MONSTER_HPP_INCLUDED
