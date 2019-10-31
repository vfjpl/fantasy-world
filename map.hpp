#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Map
{
    std::vector<sf::Sprite> tiles;

public:
    void set_texture(const sf::Texture& texture, int x, int y);

    void draw(sf::RenderWindow& window);
};

#endif // MAP_HPP_INCLUDED
