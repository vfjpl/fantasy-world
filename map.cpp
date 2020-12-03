#include "map.hpp"
#include "resourcemanager.hpp"
#include "helperfunctions.hpp"
#include <Poco/DynamicStruct.h>
#include <SFML/System/Lock.hpp>
#include <iostream>

// view-source:http://alkatria.pl/templates/client/default/js/map.js

void Map::setup(const sf::View& view)
{
    camera = view;
}

void Map::loadData_100(const Poco::DynamicAny& data, LocalPlayer& localPlayer)
{
    localPlayer.id = data["player"]["id"];
    loadData_teleport(data, localPlayer);
}

void Map::loadData_teleport(const Poco::DynamicAny& data, LocalPlayer& localPlayer)
{
    //todo better
    loadMapPositionsData(data["map_positions"], localPlayer);

    if(data["map"]["type"] == 2ul)
    {
        addSingleMapData(data["map"]);
    }
    else
    {
        for(const auto& map_chunk: data["map_data"])
            addMultiMapData(map_chunk);
    }
    for(const auto& tile: data["tiles"])
        addTile(tile);
    if(data["chests"].size())
        for(const auto& chest: data["chests"])
            addChest(chest);
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
        default:
            std::cout << i.first << '\n';
            break;
        }//end switch
    }//end for
}

void Map::movePlayer(const Poco::DynamicAny& data)
{
    unsigned long id = data["player"];
    players[id].move(data["x"], data["y"]);
    players[id].set_dir(data["dir"]);
}

void Map::addMapItem(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    map_items[id].setTexture(ResourceManager::getTexture(data["item_id"], Graphic::ITEM));
    map_items[id].set_position(data["x"], data["y"]);
}

void Map::addPlayer(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    players[id].setTexture(ResourceManager::getTexture(data["looktype"], Graphic::PLAYER));
    players[id].set_position(data["x"] + 1, data["y"] + 1);//server bug
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

struct objectsIDs Map::getObjectsIDs(sf::RenderWindow& window, sf::Vector2i point)
{
    sf::Vector2f coords = window.mapPixelToCoords(point, camera);
    return {getChestID(coords), getItemID(coords), getMonsterID(coords), getNpcID(coords), getPlayerID(coords)};
}

bool Map::isObstacle(unsigned long x, unsigned long y)
{
    if(x >= max_x || y >= max_y)
        return true;
    return obstacles[x][y];
}

void Map::moveLocalPlayer(unsigned long id, unsigned long x, unsigned long y, unsigned long dir)
{
    moveCamera(x, y);
    players[id].move(x, y);
    players[id].set_dir(dir);
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
    chests.clear();
    map_items.clear();
    monsters.clear();
    npcs.clear();
    players.clear();
}

// private

void Map::loadMapPositionsData(const Poco::DynamicAny& data, LocalPlayer& localPlayer)
{
    max_x = data["MAX_X"];
    max_y = data["MAX_Y"];
    addLocalPlayer(localPlayer, data["PLAYER_X"], data["PLAYER_Y"]);
}

void Map::addLocalPlayer(LocalPlayer& localPlayer, unsigned long x, unsigned long y)
{
    setCamera(x, y);
    players[localPlayer.id].setTexture(ResourceManager::getTexture(localPlayer.looktype, Graphic::DIRECT));
    players[localPlayer.id].set_position(x, y);
}

void Map::setCamera(unsigned long x, unsigned long y)
{
    moveCamera(x, y);
    current_camera = desired_camera;
}

void Map::moveCamera(unsigned long x, unsigned long y)
{
    desired_camera.x = (32 * x) - 16;
    desired_camera.y = (32 * y);
}

void Map::moveMonster(const Poco::DynamicAny& data)
{
    unsigned long id = data["monster"];
    monsters[id].move(data["x"], data["y"]);
    monsters[id].set_dir(data["dir"]);
}

void Map::moveNpc(const Poco::DynamicAny& data)
{
    unsigned long id = data["npc"];
    npcs[id].move(data["x"], data["y"]);
    npcs[id].set_dir(data["dir"]);
}

void Map::addSingleMapData(const Poco::DynamicAny& data)
{
    map_backgrounds.emplace_back(ResourceManager::getTexture(data["id"], Graphic::MAP_SINGLE));
}

void Map::addMultiMapData(const Poco::DynamicAny& data)
{
    map_backgrounds.emplace_back(ResourceManager::getTexture(data["source"], Graphic::MAP_MULTI));
    map_backgrounds.back().setPosition(data["x"] * 640, data["y"] * 640);
}

void Map::addTile(const Poco::DynamicAny& data)
{
    unsigned long type = data["type"];
    switch(type)
    {
    case 2:
        tiles.emplace_back(ResourceManager::getTexture(data["bg"], Graphic::DIRECT));
        break;
    case 21:
        tiles.emplace_back();
        break;
    default:
        tiles.emplace_back(ResourceManager::getTexture(data["tile"], Graphic::TILE), data["width"], data["height"]);
        break;
    }//end switch
    tiles.back().set_position(data["x"], data["y"]);
}

void Map::addChest(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    chests[id].setTexture(ResourceManager::getTexture(data["type"], data["open"] ? Graphic::CHEST_OPEN : Graphic::CHEST));
    chests[id].set_position(data["x"], data["y"]);
}

void Map::addMonster(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    monsters[id].setTexture(ResourceManager::getTexture(data["looktype"], Graphic::MONSTER), data["width"], data["height"]);
    monsters[id].set_position(data["x"], data["y"]);
}

void Map::addNpc(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    npcs[id].setTexture(ResourceManager::getTexture(data["looktype"], Graphic::NPC));
    npcs[id].set_position(data["x"], data["y"]);
}

unsigned long Map::getChestID(sf::Vector2f coords)
{
    for(auto &i: chests)
        if(i.second.contains(coords))
            return i.first;
    return 0;
}

unsigned long Map::getItemID(sf::Vector2f coords)
{
    for(auto &i: map_items)
        if(i.second.contains(coords))
            return i.first;
    return 0;
}

unsigned long Map::getMonsterID(sf::Vector2f coords)
{
    for(auto &i: monsters)
        if(i.second.contains(coords))
            return i.first;
    return 0;
}

unsigned long Map::getNpcID(sf::Vector2f coords)
{
    for(auto &i: npcs)
        if(i.second.contains(coords))
            return i.first;
    return 0;
}

unsigned long Map::getPlayerID(sf::Vector2f coords)
{
    for(auto &i: players)
        if(i.second.contains(coords))
            return i.first;
    return 0;
}
