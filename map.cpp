#include "map.hpp"
#include "mapobject.hpp"
#include "chest.hpp"
#include "mapitem.hpp"
#include "monster.hpp"
#include "npc.hpp"
#include "player.hpp"
#include "tile.hpp"
#include "resourcemanager.hpp"
#include "localplayer.hpp"
#include "helperfunctions.hpp"
#include <Poco/DynamicStruct.h>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <iostream>

// view-source:http://alkatria.pl/templates/client/default/js/map.js

namespace
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

void moveCamera(unsigned long x, unsigned long y)
{
    desired_camera.x = (x * 32) - 16;
    desired_camera.y = (y * 32);
}

void setCamera(unsigned long x, unsigned long y)
{
    moveCamera(x, y);
    current_camera = desired_camera;
}

bool isMapItem(sf::Vector2f coords)
{
    for(auto& i: map_items)
        if(i.second.contains(coords))
            return true;
    return false;
}

bool isTile(sf::Vector2f coords)
{
    for(auto& i: tiles)
        if(i.contains(coords))
            return true;
    return false;
}

unsigned long getPlayerIDf(sf::Vector2f coords)
{
    for(auto& i: players)
        if(i.second.contains(coords))
            return i.first;
    return 0;
}

unsigned long getNpcIDf(sf::Vector2f coords)
{
    for(auto& i: npcs)
        if(i.second.contains(coords))
            return i.first;
    return 0;
}

unsigned long getMonsterIDf(sf::Vector2f coords)
{
    for(auto& i: monsters)
        if(i.second.contains(coords))
            return i.first;
    return 0;
}

unsigned long getChestIDf(sf::Vector2f coords)
{
    for(auto& i: chests)
        if(i.second.contains(coords))
            return i.first;
    return 0;
}

void moveNpc(const Poco::DynamicAny& data)
{
    unsigned long id = data["npc"];
    npcs[id].move(data["x"], data["y"]);
    npcs[id].setDir(data["dir"]);
}

void moveMonster(const Poco::DynamicAny& data)
{
    unsigned long id = data["monster"];
    monsters[id].move(data["x"], data["y"]);
    monsters[id].setDir(data["dir"]);
}

void addNpc(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    npcs[id].setTexture(ResourceManager::getTexture(data["looktype"], Graphic::NPC));
    npcs[id].setPosition(data["x"], data["y"], data["can_walk"]);
}

void addMonster(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    monsters[id].setTexture(ResourceManager::getTexture(data["looktype"], Graphic::MONSTER), data["width"], data["height"]);
    monsters[id].setPosition(data["x"], data["y"]);
}

void addChest(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    chests[id].setTexture(ResourceManager::getTexture(data["type"], data["open"] ? Graphic::CHEST_OPEN : Graphic::CHEST));
    chests[id].setPosition(data["x"], data["y"]);
}

void addMapObject(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    map_objects[id].setTexture(ResourceManager::getTexture(data["file"], Graphic::GAME_OBJECT), data["width"], data["height"]);
    map_objects[id].setPosition(data["x"], data["y"]);
}

void addTile(const Poco::DynamicAny& data)
{
    switch((unsigned long)data["type"])
    {
    case 2://door
    {
        const sf::Texture& texture = ResourceManager::getTexture(data["bg"], Graphic::DIRECT);
        unsigned long x = data["x"];
        unsigned long y = data["y"];
        sf::Lock lock(mutex);
        tiles.emplace_back(texture);
        tiles.back().setPosition(x, y);
        break;
    }
    case 21://well
    {
        unsigned long x = data["x"];
        unsigned long y = data["y"];
        sf::Lock lock(mutex);
        tiles.emplace_back();
        tiles.back().setPosition(x, y);
        break;
    }
    default:
    {
        const sf::Texture& texture = ResourceManager::getTexture(data["tile"], Graphic::GAME_OBJECT);
        unsigned long width = data["width"];
        unsigned long height = data["height"];
        unsigned long x = data["x"];
        unsigned long y = data["y"];
        sf::Lock lock(mutex);
        tiles.emplace_back(texture, width, height);
        tiles.back().setPosition(x, y);
        break;
    }
    }//end switch
}

void addMultiMapData(const Poco::DynamicAny& data)
{
    const sf::Texture& texture = ResourceManager::getTexture(data["source"], Graphic::MAP_MULTI);
    unsigned long x = data["x"] * 640ul;
    unsigned long y = data["y"] * 640ul;
    sf::Lock lock(mutex);
    map_backgrounds.emplace_back(texture);
    map_backgrounds.back().setPosition(x, y);
}

void addSingleMapData(const Poco::DynamicAny& data)
{
    const sf::Texture& texture = ResourceManager::getTexture(data["id"], Graphic::MAP_SINGLE);
    sf::Lock lock(mutex);
    map_backgrounds.emplace_back(texture);
}

void loadMapPositionData(const Poco::DynamicAny& data)
{
    max_x = data["MAX_X"];
    max_y = data["MAX_Y"];
    LocalPlayer::x = data["PLAYER_X"];
    LocalPlayer::y = data["PLAYER_Y"];
    setCamera(LocalPlayer::x, LocalPlayer::y);
    players[LocalPlayer::id].setTexture(ResourceManager::getTexture(LocalPlayer::looktype, Graphic::DIRECT));
    players[LocalPlayer::id].setPosition(LocalPlayer::x, LocalPlayer::y);
}
}//end namespace


void Map::updateWindowSize(float width, float height)
{
    camera.setSize(width, height);
}

void Map::moveLocalPlayer(const Poco::DynamicAny& data)
{
    LocalPlayer::x = data["x"];
    LocalPlayer::y = data["y"];
    moveCamera(LocalPlayer::x, LocalPlayer::y);
    players[LocalPlayer::id].move(LocalPlayer::x, LocalPlayer::y);
    players[LocalPlayer::id].setDir(data["dir"]);
}

void Map::loadData_100(const Poco::DynamicAny& data)
{
    LocalPlayer::id = data["player"]["id"];
    loadData_teleport(data);
}

void Map::loadData_teleport(const Poco::DynamicAny& data)
{
    obstacles.swap((Poco::DynamicAny&)data["obstacles"]);
    loadMapPositionData(data["map_positions"]);

    //todo better
    if(data["map"]["type"] == 2ul)
    {
        addSingleMapData(data["map"]);
    }
    else
    {
        for(const auto& map_chunk: data["map_data"])
            addMultiMapData(map_chunk);
    }

    if(data["tiles"].size())
        for(const auto& tile: data["tiles"])
            addTile(tile);
    if(data["map_objects"].size())
        for(const auto& map_object: data["map_objects"])
            addMapObject(map_object);
    if(data["chests"].size())
        for(const auto& chest: data["chests"])
            addChest(chest);
    if(data["map_items"].size())
        for(const auto& map_item: data["map_items"])
            addMapItem(map_item);
    if(data["monsters"].size())
        for(const auto& monster: data["monsters"])
            addMonster(monster);
    if(data["npcs"].size())
        for(const auto& npc: data["npcs"])
            addNpc(npc);
    if(data["players"].size())
        for(const auto& player: data["players"])
            addPlayer(player);
}

void Map::updateMapData(const Poco::DynamicAny& data)
{
    for(const auto& i: data.extract<Poco::DynamicStruct>())
    {
        switch(str2int(i.first))
        {
        case char2int("moves"):
            for(const auto& monster: i.second)
                moveMonster(monster);
            break;
        case char2int("npc_moves"):
            for(const auto& npc: i.second)
                moveNpc(npc);
            break;
        case char2int("respawns"):
            for(const auto& monster: i.second)
                addMonster(monster);
            break;
        case char2int("damages"):
            break;
        case char2int("spells"):
            break;
        case char2int("yells"):
            break;
        case char2int("monster_yells"):
            break;
        default:
            std::cout << i.first << '\n';
            break;
        }//end switch
    }//end for
}

void Map::moveOutfit(const Poco::DynamicAny& data)
{
    unsigned long id = data["player"];
    players[id].setDir(data["dir"]);
}

void Map::movePlayer(const Poco::DynamicAny& data)
{
    unsigned long id = data["player"];
    players[id].move(data["x"], data["y"]);
    players[id].setDir(data["dir"]);
}

void Map::addMapItem(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    map_items[id].setTexture(ResourceManager::getTexture(data["item_id"], Graphic::ITEM));
    map_items[id].setPosition(data["x"], data["y"]);
}

void Map::addPlayer(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    players[id].setTexture(ResourceManager::getTexture(data["looktype"], Graphic::PLAYER));
    players[id].setPosition(data["x"] + 1ul, data["y"] + 1ul);//server bug
}

void Map::deleteMapItem(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    map_items.erase(id);
}

void Map::deleteMonster(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    monsters.erase(id);
}

void Map::deletePlayer(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    players.erase(id);
}

void Map::openChest(const Poco::DynamicAny& data)
{
    //todo better
    if(data["chest"].size())
    {
        unsigned long id = data["chest"];
        chests[id].setTexture(ResourceManager::getTexture(std::to_string(1ul), Graphic::CHEST_OPEN));
    }
}

void Map::updateTile(const Poco::DynamicAny& data)
{
    unsigned long x = data["x"];
    unsigned long y = data["y"];
    //todo better
    for(auto& i: tiles)
    {
        if(i.isOnPosition(x, y))
        {
            i.setTexture(ResourceManager::getTexture(data["file"], Graphic::GAME_OBJECT));
            break;
        }
    }
}

MapClickData Map::mapMouseClick(sf::RenderWindow& window, sf::Vector2i point)
{
    sf::Vector2f coords = window.mapPixelToCoords(point, camera);
    return {getChestIDf(coords), getMonsterIDf(coords), getNpcIDf(coords), getPlayerIDf(coords),
            (unsigned long)std::ceil(coords.x/32), (unsigned long)std::ceil(coords.y/32),
            isTile(coords), isMapItem(coords)};
}

unsigned long Map::getMonsterID(unsigned long x, unsigned long y)
{
    for(auto& i: monsters)
        if(i.second.isOnPosition(x, y))
            return i.first;
    return 0;
}

bool Map::isObstacle(unsigned long x, unsigned long y)
{
    if(x >= max_x || y >= max_y)
        return true;
    return obstacles[x][y];
}

bool Map::isNpc(unsigned long x, unsigned long y)
{
    for(auto& i: npcs)
        if(i.second.isOnPosition(x, y))
            return true;
    return false;
}

void Map::draw(sf::RenderWindow& window)
{
    sf::Lock lock(mutex);

    current_camera += clamp(desired_camera - current_camera);
    camera.setCenter(current_camera.x, current_camera.y);
    window.setView(camera);

    for(auto& i: map_backgrounds)
        window.draw(i);
    for(auto& i: tiles)
        i.draw(window);
    for(auto& i: map_objects)
        i.second.draw(window);
    for(auto& i: chests)
        i.second.draw(window);
    for(auto& i: map_items)
        i.second.draw(window);
    for(auto& i: monsters)
        i.second.draw(window);
    for(auto& i: npcs)
        i.second.draw(window);
    for(auto& i: players)
        i.second.draw(window);
}

void Map::clear()
{
    sf::Lock lock(mutex);

    map_backgrounds.clear();
    tiles.clear();
    map_objects.clear();
    chests.clear();
    map_items.clear();
    monsters.clear();
    npcs.clear();
    players.clear();
}
