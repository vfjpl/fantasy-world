#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "door.hpp"
#include "chest.hpp"
#include "item.hpp"
#include "monster.hpp"
#include "npc.hpp"
#include "player.hpp"
#include <Poco/DynamicAny.h>
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
    //24
    std::vector<sf::Sprite> map_backgrounds;
    std::vector<Door> doors;
    //8
    Poco::DynamicAny obstacles;
    sf::Vector2i current_position;
    sf::Vector2i current_camera;
    sf::Vector2i desired_camera;
    sf::Mutex mutex;

public:
    void setDefaultCamera(const sf::View& view);

    void firstLoadMapData(const Poco::DynamicAny& data);
    void loadMapData(const Poco::DynamicAny& data);
    void updateMapData(const Poco::DynamicAny& data);

    void movePlayer(const Poco::DynamicAny& data);
    void addMapItem(const Poco::DynamicAny& data);
    void addPlayer(const Poco::DynamicAny& data);
    void deleteMapItem(const Poco::DynamicAny& data);
    void deleteMonster(const Poco::DynamicAny& data);
    void deletePlayer(const Poco::DynamicAny& data);

    void pointToObjectsIDs(sf::RenderWindow& window, sf::Vector2i point);
    bool moveDirIfPossible(unsigned long dir);
    void draw(sf::RenderWindow& window);

private:
    void moveCamera(unsigned long x, unsigned long y);
    void setCamera(unsigned long x, unsigned long y);
    bool moveToIfPossible(unsigned long x, unsigned long y);
    void setPosition(unsigned long x, unsigned long y);

    void moveMonster(const Poco::DynamicAny& data);
    void moveNpc(const Poco::DynamicAny& data);
    void addSingleMapData(const Poco::DynamicAny& data);
    void addMultiMapData(const Poco::DynamicAny& data);
    void addMultiMapData(const sf::Texture& texture, unsigned long x, unsigned long y);
    void addTile(const Poco::DynamicAny& data);
    void addChest(const Poco::DynamicAny& data);
    void addMonster(const Poco::DynamicAny& data);
    void addNpc(const Poco::DynamicAny& data);

    unsigned long getChestID(sf::Vector2f coords);
    unsigned long getItemID(sf::Vector2f coords);
    unsigned long getMonsterID(sf::Vector2f coords);
    unsigned long getNpcID(sf::Vector2f coords);
    unsigned long getPlayerID(sf::Vector2f coords);

    void clear();
};

#endif // MAP_HPP_INCLUDED
