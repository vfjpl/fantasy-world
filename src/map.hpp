#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <Poco/DynamicAny.h>
#include <SFML/Graphics/RenderWindow.hpp>

struct MapClickData
{
    unsigned long chestID;
    unsigned long monsterID;
    unsigned long npcID;
    unsigned long playerID;

    unsigned long x;
    unsigned long y;

    bool tile;
    bool item;
};

namespace Map
{
void updateWindowSize(float width, float height);

void moveLocalPlayer(const Poco::DynamicAny& data);
void loadData_100(const Poco::DynamicAny& data);
void loadData_teleport(const Poco::DynamicAny& data);
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
unsigned long getMonsterID(unsigned long x, unsigned long y);
bool isObstacle(unsigned long x, unsigned long y);
bool isNpc(unsigned long x, unsigned long y);
void draw(sf::RenderWindow& window);
void clear();
};

#endif // MAP_HPP_INCLUDED
