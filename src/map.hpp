#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "mapobject.hpp"
#include "chest.hpp"
#include "mapitem.hpp"
#include "monster.hpp"
#include "npc.hpp"
#include "player.hpp"
#include "tile.hpp"
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

    bool tile;
    bool item;
};

class Map_t
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
	Poco::DynamicAny obstacles;
	sf::Vector2i current_camera;
	sf::Vector2i desired_camera;
	sf::Mutex mutex;
	unsigned long max_x;
	unsigned long max_y;


	void moveCamera(unsigned long x, unsigned long y);
	void setCamera(unsigned long x, unsigned long y);
	bool isMapItem(sf::Vector2f coords);
	bool isTile(sf::Vector2f coords);
	unsigned long getPlayerIDf(sf::Vector2f coords);
	unsigned long getNpcIDf(sf::Vector2f coords);
	unsigned long getMonsterIDf(sf::Vector2f coords);
	unsigned long getChestIDf(sf::Vector2f coords);
	void moveNpc(const Poco::DynamicAny& data);
	void moveMonster(const Poco::DynamicAny& data);
	void addNpc(const Poco::DynamicAny& data);
	void addMonster(const Poco::DynamicAny& data);
	void addChest(const Poco::DynamicAny& data);
	void addMapObject(const Poco::DynamicAny& data);
	void addTile(const Poco::DynamicAny& data);
	void addMultiMapData(const Poco::DynamicAny& data);
	void addSingleMapData(const Poco::DynamicAny& data);
	void loadMapPositionData(const Poco::DynamicAny& data);

public:
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

extern Map_t Map;

#endif // MAP_HPP_INCLUDED
