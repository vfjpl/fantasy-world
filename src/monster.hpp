#ifndef MONSTER_HPP_INCLUDED
#define MONSTER_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Monster
{
    //288
    sf::Sprite sprite;
    //16
    sf::IntRect rect;
    //8
    sf::Vector2i current_px;
    sf::Vector2i desired_px;
    long position_x;
    long position_y;
    long steps;
    long frames_x;
    long frames_y;

public:
    void setTexture(const sf::Texture& texture, long width, long height);
    void setDir(long dir);
    void setPosition(long x, long y);
    void move(long x, long y);

    bool isOnPosition(long x, long y);
    bool contains(sf::Vector2f coords);
    void draw(sf::RenderWindow& window);
};

#endif // MONSTER_HPP_INCLUDED
