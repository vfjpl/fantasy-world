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
    //168
    sf::View camera;
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
    sf::Vector2i current_camera;
    sf::Vector2i desired_camera;
    sf::Mutex mutex;

public:
    void initDefaultCamera(const sf::View& view);
    void loadMapData(const Poco::DynamicStruct& data);
    void updateMapData(const Poco::DynamicStruct& data);

    void addMapItem(const Poco::DynamicStruct& data);
    void addPlayer(const Poco::DynamicStruct& data);
    void movePlayer(const Poco::DynamicStruct& data);
    void moveMe(const Poco::DynamicStruct& data);
    void deleteMapItem(const Poco::DynamicStruct& data);
    void deleteMonster(const Poco::DynamicStruct& data);
    void deletePlayer(const Poco::DynamicStruct& data);

    void draw(sf::RenderWindow& window);

private:
    void loadPlayerData(const Poco::DynamicStruct& data);
    void loadMapPositions(const Poco::DynamicStruct& data);

    void moveCamera(int x, int y);
    void setCamera(int x, int y);

    void addMap(const Poco::DynamicStruct& data);
    void addMapData(const Poco::DynamicStruct& data);
    void addMapData(const sf::Texture& texture, int x, int y);
    void addTile(const Poco::DynamicStruct& data);
    void addChest(const Poco::DynamicStruct& data);
    void addMonster(const Poco::DynamicStruct& data);
    void addNpc(const Poco::DynamicStruct& data);
    void moveMonster(const Poco::DynamicStruct& data);
    void moveNpc(const Poco::DynamicStruct& data);

    void clear();
};

#endif // MAP_HPP_INCLUDED
