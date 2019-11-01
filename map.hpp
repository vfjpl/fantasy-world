#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "monster.hpp"
#include <map>

class Map
{
public:
    std::map<int, Monster> monsters;
    std::vector<sf::Sprite> tiles;

    void set_texture(const sf::Texture& texture, int x, int y);
    void draw(sf::RenderWindow& window);
};

#endif // MAP_HPP_INCLUDED
