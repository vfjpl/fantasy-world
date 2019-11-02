#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class NPC
{
    sf::Sprite sprite;
    sf::IntRect rect;
    sf::Vector2i position;

public:
    void set_texture(const sf::Texture& texture);
    void set_position(int x, int y);
    void move(int x, int y);
    void draw(sf::RenderWindow& window);
};

#endif // NPC_HPP_INCLUDED
