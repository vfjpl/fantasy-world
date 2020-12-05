#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Player
{
    //288
    sf::Sprite sprite;
    //16
    sf::IntRect rect;
    //8
    sf::Vector2i current_px;
    sf::Vector2i desired_px;
    unsigned long steps;
    unsigned long frames_x;
    unsigned long frames_y;

public:
    void setTexture(const sf::Texture& texture);
    void setDir(unsigned long dir);
    void setPosition(unsigned long x, unsigned long y);
    void move(unsigned long x, unsigned long y);

    bool contains(sf::Vector2f coords);
    void draw(sf::RenderWindow& window);
};

#endif // PLAYER_HPP_INCLUDED
