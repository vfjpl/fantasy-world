#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "door.hpp"
#include "chest.hpp"
#include "item.hpp"
#include "monster.hpp"
#include "npc.hpp"
#include "player.hpp"
#include <Poco/DynamicStruct.h>
#include <SFML/System/Mutex.hpp>

class Map
{
    //48
    std::map<int, Chest> chests;
    std::map<int, Item> map_items;
    std::map<int, Monster> monsters;
    std::map<int, Npc> npcs;
    std::map<int, Player> players;
    //24
    std::vector<sf::Sprite> map_data;
    std::vector<Door> doors;
    //8
    sf::Mutex mutex;

public:
    void loadData(const Poco::DynamicStruct& data);
    void updateData(const Poco::DynamicStruct& data);

    void addMapItem(const Poco::DynamicStruct& data);
    void addPlayer(const Poco::DynamicStruct& data);

    void moveMonster(const Poco::DynamicStruct& data);
    void moveNpc(const Poco::DynamicStruct& data);
    void movePlayer(const Poco::DynamicStruct& data);
    void deleteMapItem(const Poco::DynamicStruct& data);
    void deleteMonster(const Poco::DynamicStruct& data);
    void deletePlayer(const Poco::DynamicStruct& data);

    void draw(sf::RenderWindow& window);

private:
    void loadMap(const Poco::DynamicStruct& data);
    void updateMap(const Poco::DynamicStruct& data);
    void addMap(const Poco::DynamicStruct& data);
    void addMapData(const Poco::DynamicStruct& data);
    void addMapData(const sf::Texture& texture, int x, int y);
    void addTile(const Poco::DynamicStruct& data);
    void addChest(const Poco::DynamicStruct& data);
    void addMonster(const Poco::DynamicStruct& data);
    void addNpc(const Poco::DynamicStruct& data);
    void clear();
};

#endif // MAP_HPP_INCLUDED
