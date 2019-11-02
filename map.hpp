#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "monster.hpp"
#include "npc.hpp"
#include "player.hpp"
#include <SFML/System/Mutex.hpp>
#include <map>

class Map
{
public:
    std::map<int, Monster> monsters;
    std::map<int, NPC> NPCs;
    std::vector<sf::Sprite> tiles;
    Player player;
    sf::Mutex mutex;

    void set_texture(const sf::Texture& texture, int x, int y);
    void clear();
    void draw(sf::RenderWindow& window);
};

#endif // MAP_HPP_INCLUDED
