#ifndef MONSTER_HPP_INCLUDED
#define MONSTER_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Monster
{
    sf::Sprite sprite;
    sf::IntRect rect;
    sf::Vector2i position;

public:
    void setTexture(const sf::Texture& texture, int width, int height);
    void set_dir(int dir);
    void set_position(int x, int y);
    void move(int x, int y);
    void draw(sf::RenderWindow& window);
};

#endif // MONSTER_HPP_INCLUDED
