#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "tile.hpp"
#include "mapobject.hpp"
#include "chest.hpp"
#include "mapitem.hpp"
#include "monster.hpp"
#include "npc.hpp"
#include "player.hpp"
#include "localplayer.hpp"
#include <Poco/DynamicAny.h>
#include <SFML/System/Mutex.hpp>

struct MapClickData
{
    unsigned long chestID;
    unsigned long monsterID;
    unsigned long npcID;
    unsigned long playerID;

    unsigned long x;
    unsigned long y;

    bool item;
    bool tile;
};

class Map
{
    //168
    sf::View camera;
    //48
    std::map<unsigned long, MapObject> map_objects;
    std::map<unsigned long, Chest> chests;
    std::map<unsigned long, MapItem> map_items;
    std::map<unsigned long, Monster> monsters;
    std::map<unsigned long, Npc> npcs;
    std::map<unsigned long, Player> players;
    //24
    std::vector<sf::Sprite> map_backgrounds;
    std::vector<Tile> tiles;
    //8
    sf::Mutex mutex;
    sf::Vector2i current_camera;
    sf::Vector2i desired_camera;
    Poco::DynamicAny obstacles;
    unsigned long max_x;
    unsigned long max_y;

public:
    void updateWindowSize(float width, float height);

    void moveLocalPlayer(const Poco::DynamicAny& data, LocalPlayer& localPlayer);
    void loadData_100(const Poco::DynamicAny& data, LocalPlayer& localPlayer);
    void loadData_teleport(const Poco::DynamicAny& data, LocalPlayer& localPlayer);
    void updateMapData(const Poco::DynamicAny& data);

    void moveOutfit(const Poco::DynamicAny& data);
    void movePlayer(const Poco::DynamicAny& data);
    void addMapItem(const Poco::DynamicAny& data);
    void addPlayer(const Poco::DynamicAny& data);
    void deleteMapItem(const Poco::DynamicAny& data);
    void deleteMonster(const Poco::DynamicAny& data);
    void deletePlayer(const Poco::DynamicAny& data);
    void openChest(const Poco::DynamicAny& data);
    void updateTile(const Poco::DynamicAny& data);

    MapClickData mapMouseClick(sf::RenderWindow& window, sf::Vector2i point);
    unsigned long getMonsterIDOnPosition(unsigned long x, unsigned long y);
    bool isObstacle(unsigned long x, unsigned long y);
    bool isNpcOnPosition(unsigned long x, unsigned long y);
    void draw(sf::RenderWindow& window);
    void clear();

private:
    void loadMapPositionData(const Poco::DynamicAny& data, LocalPlayer& localPlayer);
    void setCamera(unsigned long x, unsigned long y);
    void moveCamera(unsigned long x, unsigned long y);

    void moveMonster(const Poco::DynamicAny& data);
    void moveNpc(const Poco::DynamicAny& data);
    void addSingleMapData(const Poco::DynamicAny& data);
    void addMultiMapData(const Poco::DynamicAny& data);
    void addTile(const Poco::DynamicAny& data);
    void addMapObject(const Poco::DynamicAny& data);
    void addChest(const Poco::DynamicAny& data);
    void addMonster(const Poco::DynamicAny& data);
    void addNpc(const Poco::DynamicAny& data);

    unsigned long getChestID(sf::Vector2f coords);
    unsigned long getMonsterID(sf::Vector2f coords);
    unsigned long getNpcID(sf::Vector2f coords);
    unsigned long getPlayerID(sf::Vector2f coords);
    bool isMapItem(sf::Vector2f coords);
    bool isTile(sf::Vector2f coords);
};

#endif // MAP_HPP_INCLUDED
