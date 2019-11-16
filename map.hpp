#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "monster.hpp"
#include "npc.hpp"
#include "player.hpp"
#include <SFML/System/Mutex.hpp>
#include <map>

class Map
{
    sf::Mutex mutex;

public:
    std::map<int, Monster> monsters;
    std::map<int, Npc> npcs;
    std::map<int, Player> players;
    std::vector<sf::Sprite> tiles;
    Player player;

    void setTexture(const sf::Texture& texture, int x, int y);
    void clear();
    void draw(sf::RenderWindow& window);
};

#endif // MAP_HPP_INCLUDED
