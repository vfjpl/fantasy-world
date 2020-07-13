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
    std::map<unsigned long, Chest> chests;
    std::map<unsigned long, Item> map_items;
    std::map<unsigned long, Monster> monsters;
    std::map<unsigned long, Npc> npcs;
    std::map<unsigned long, Player> players;
    //32
    std::string player_looktype;
    //24
    std::vector<sf::Sprite> map_data;
    std::vector<Door> doors;
    //8
    sf::Vector2i current_camera;
    sf::Vector2i desired_camera;
    sf::Mutex mutex;
    unsigned long player_id;

public:
    void setDefaultCamera(const sf::View& view);
    void setPlayerLooktype(const std::string& looktype);

    void initData(const Poco::DynamicStruct& data);
    void loadMapData(const Poco::DynamicStruct& data);
    void updateMapData(const Poco::DynamicStruct& data);

    void movePlayer(const Poco::DynamicStruct& data);
    void moveMe(const Poco::DynamicStruct& data);
    void addMapItem(const Poco::DynamicStruct& data);
    void addPlayer(const Poco::DynamicStruct& data);
    void deleteMapItem(const Poco::DynamicStruct& data);
    void deleteMonster(const Poco::DynamicStruct& data);
    void deletePlayer(const Poco::DynamicStruct& data);

    unsigned long getIDs(sf::RenderWindow& window, sf::Vector2i point);
    void draw(sf::RenderWindow& window);

private:
    void moveCamera(unsigned long x, unsigned long y);
    void setCamera(unsigned long x, unsigned long y);

    void parsePlayerData(const Poco::DynamicStruct& data);
    void parseMapPositionsData(const Poco::DynamicStruct& data);

    void moveMonster(const Poco::DynamicStruct& data);
    void moveNpc(const Poco::DynamicStruct& data);
    void addMap(const Poco::DynamicStruct& data);
    void addMapData(const Poco::DynamicStruct& data);
    void addMapData(const sf::Texture& texture, unsigned long x, unsigned long y);
    void addTile(const Poco::DynamicStruct& data);
    void addChest(const Poco::DynamicStruct& data);
    void addMonster(const Poco::DynamicStruct& data);
    void addNpc(const Poco::DynamicStruct& data);

    unsigned long getChestID(sf::Vector2f coords);
    unsigned long getItemID(sf::Vector2f coords);
    unsigned long getMonsterID(sf::Vector2f coords);
    unsigned long getNpcID(sf::Vector2f coords);
    unsigned long getPlayerID(sf::Vector2f coords);

    void clear();
};

#endif // MAP_HPP_INCLUDED
