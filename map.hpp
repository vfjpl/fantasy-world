#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "tile.hpp"
#include "mapobject.hpp"
#include "chest.hpp"
#include "mapitem.hpp"
#include "monster.hpp"
#include "npc.hpp"
#include "player.hpp"
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
    static sf::View camera;
    //48
    static std::map<unsigned long, MapObject> map_objects;
    static std::map<unsigned long, Chest> chests;
    static std::map<unsigned long, MapItem> map_items;
    static std::map<unsigned long, Monster> monsters;
    static std::map<unsigned long, Npc> npcs;
    static std::map<unsigned long, Player> players;
    //24
    static std::vector<sf::Sprite> map_backgrounds;
    static std::vector<Tile> tiles;
    //8
    static sf::Mutex mutex;
    static sf::Vector2i current_camera;
    static sf::Vector2i desired_camera;
    static Poco::DynamicAny obstacles;
    static unsigned long max_x;
    static unsigned long max_y;

public:
    static void updateWindowSize(float width, float height);

    static void moveLocalPlayer(const Poco::DynamicAny& data);
    static void loadData_100(const Poco::DynamicAny& data);
    static void loadData_teleport(const Poco::DynamicAny& data);
    static void updateMapData(const Poco::DynamicAny& data);

    static void moveOutfit(const Poco::DynamicAny& data);
    static void movePlayer(const Poco::DynamicAny& data);
    static void addMapItem(const Poco::DynamicAny& data);
    static void addPlayer(const Poco::DynamicAny& data);
    static void deleteMapItem(const Poco::DynamicAny& data);
    static void deleteMonster(const Poco::DynamicAny& data);
    static void deletePlayer(const Poco::DynamicAny& data);
    static void openChest(const Poco::DynamicAny& data);
    static void updateTile(const Poco::DynamicAny& data);

    static MapClickData mapMouseClick(sf::RenderWindow& window, sf::Vector2i point);
    static unsigned long getMonsterID(unsigned long x, unsigned long y);
    static bool isObstacle(unsigned long x, unsigned long y);
    static bool isNpc(unsigned long x, unsigned long y);
    static void draw(sf::RenderWindow& window);
    static void clear();

private:
    static void loadMapPositionData(const Poco::DynamicAny& data);
    static void setCamera(unsigned long x, unsigned long y);
    static void moveCamera(unsigned long x, unsigned long y);

    static void moveMonster(const Poco::DynamicAny& data);
    static void moveNpc(const Poco::DynamicAny& data);
    static void addSingleMapData(const Poco::DynamicAny& data);
    static void addMultiMapData(const Poco::DynamicAny& data);
    static void addTile(const Poco::DynamicAny& data);
    static void addMapObject(const Poco::DynamicAny& data);
    static void addChest(const Poco::DynamicAny& data);
    static void addMonster(const Poco::DynamicAny& data);
    static void addNpc(const Poco::DynamicAny& data);

    static unsigned long getChestID(sf::Vector2f coords);
    static unsigned long getMonsterID(sf::Vector2f coords);
    static unsigned long getNpcID(sf::Vector2f coords);
    static unsigned long getPlayerID(sf::Vector2f coords);
    static bool isMapItem(sf::Vector2f coords);
    static bool isTile(sf::Vector2f coords);
};

#endif // MAP_HPP_INCLUDED
