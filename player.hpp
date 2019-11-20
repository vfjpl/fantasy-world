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

public:
    void setTexture(const sf::Texture& texture);
    void set_dir(int dir);
    void move(int x, int y);
    void set_position(int x, int y);
    void set_position_server_bug(int x, int y);
    void draw(sf::RenderWindow& window);
};

#endif // PLAYER_HPP_INCLUDED
