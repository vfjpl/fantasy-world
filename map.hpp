#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "door.hpp"
#include "chest.hpp"
#include "item.hpp"
#include "monster.hpp"
#include "npc.hpp"
#include "player.hpp"
#include <SFML/System/Mutex.hpp>
#include <map>

class Map
{
    sf::Mutex mutex;

public:
    std::map<int, Chest> chests;
    std::map<int, Item> items;
    std::map<int, Monster> monsters;
    std::map<int, Npc> npcs;
    std::map<int, Player> players;
    std::vector<sf::Sprite> tiles;
    std::vector<Door> doors;
    Player player;

    void setTexture(const sf::Texture& texture, int x, int y);
    void clear();
    void draw(sf::RenderWindow& window);
};

#endif // MAP_HPP_INCLUDED
