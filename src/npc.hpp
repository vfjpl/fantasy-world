#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Npc
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
    bool npc_can_walk;

public:
    void setTexture(const sf::Texture& texture);
    void setDir(long dir);
    void setPosition(long x, long y, bool can_walk);
    void move(long x, long y);

    bool isOnPosition(long x, long y);
    bool contains(sf::Vector2f coords);
    void draw(sf::RenderWindow& window);
};

#endif // NPC_HPP_INCLUDED
