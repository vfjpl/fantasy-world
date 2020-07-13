#include "map.hpp"
#include "resourcemanager.hpp"
#include "helperfunctions.hpp"
#include <SFML/System/Lock.hpp>
#include <iostream>

//view-source:http://fantasy-world.pl/templates/client/default/js/map.js

void Map::setDefaultCamera(const sf::View& view)
{
    camera = view;
}

void Map::setPlayerLooktype(const std::string& looktype)
{
    player_looktype = looktype;
}

void Map::initData(const Poco::DynamicAny& data)
{
    parsePlayerData(data["player"]);
    loadMapData(data);
}

void Map::loadMapData(const Poco::DynamicAny& data)
{
    clear();
    parseMapPositionsData(data["map_positions"]);

    if(true)
    {
        const auto& map_data = data["map_data"];
        for(sf::Uint8 i = 0; i < map_data.size(); ++i)
            addMapData(map_data[i]);
    }
    else
    {
        addMap(data["map"]);
    }

    const auto& tiles = data["tiles"];
    for(sf::Uint8 i = 0; i < tiles.size(); ++i)
        addTile(tiles[i]);

    const auto& chests = data["chests"];
    for(sf::Uint8 i = 0; i < chests.size(); ++i)
        addChest(chests[i]);

    if(true)
    {
        const auto& map_items = data["map_items"];
        for(sf::Uint8 i = 0; i < map_items.size(); ++i)
            addMapItem(map_items[i]);
    }

    const auto& monsters = data["monsters"];
    for(sf::Uint8 i = 0; i < monsters.size(); ++i)
        addMonster(monsters[i]);

    const auto& npcs = data["npcs"];
    for(sf::Uint8 i = 0; i < npcs.size(); ++i)
        addNpc(npcs[i]);

    const auto& players = data["players"];
    for(sf::Uint8 i = 0; i < players.size(); ++i)
        addPlayer(players[i]);
}

void Map::updateMapData(const Poco::DynamicAny& data)
{
    for(const auto& i: data)
    {
        switch(str2int(i))
        {
        case char2int("moves"):
        {
            const auto& monsters = i;
            for(sf::Uint8 i = 0; i < monsters.size(); ++i)
                moveMonster(monsters[i]);
            break;
        }
        case char2int("npc_moves"):
        {
            const auto& npcs = i;
            for(sf::Uint8 i = 0; i < npcs.size(); ++i)
                moveNpc(npcs[i]);
            break;
        }
        case char2int("respawns"):
        {
            const auto& monsters = i;
            for(sf::Uint8 i = 0; i < monsters.size(); ++i)
                addMonster(monsters[i]);
            break;
        }
        case char2int("damages"):
        {
            break;
        }
        case char2int("spells"):
        {
            break;
        }
        case char2int("yells"):
        {
            break;
        }
        default:
        {
            std::cout << i.toString() << '\n';
            break;
        }
        }//end switch
    }//end for
}

void Map::movePlayer(const Poco::DynamicAny& data)
{
    unsigned long id = data["player"];
    players[id].move(data["x"], data["y"]);
    players[id].set_dir(data["dir"]);
}

void Map::moveMe(const Poco::DynamicAny& data)
{
    unsigned long x = data["x"];
    unsigned long y = data["y"];
    moveCamera(x, y);
    players[player_id].move(x, y);
    players[player_id].set_dir(data["dir"]);
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
    unsigned long x = data["x"];
    unsigned long y = data["y"];
    players[id].setTexture(ResourceManager::getTexture(data["looktype"], Graphic::PLAYER));
    players[id].set_position(x + 1, y + 1);//server bug
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

unsigned long Map::getIDs(sf::RenderWindow& window, sf::Vector2i point)
{
    sf::Vector2f coords = window.mapPixelToCoords(point, camera);

    unsigned long chestID = getChestID(coords);
    unsigned long itemID = getItemID(coords);
    unsigned long monsterID = getMonsterID(coords);
    unsigned long npcID = getNpcID(coords);
    unsigned long playerID = getPlayerID(coords);

    return 0;
}

void Map::draw(sf::RenderWindow& window)
{
    sf::Lock lock(mutex);

    current_camera += clamp(desired_camera - current_camera);
    camera.setCenter(current_camera.x, current_camera.y);
    window.setView(camera);

    for(auto& i: map_data)
        window.draw(i);
    for(auto& i: doors)
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

// private

void Map::moveCamera(unsigned long x, unsigned long y)
{
    desired_camera.x = (32 * x) - 16;
    desired_camera.y = (32 * y);
}

void Map::setCamera(unsigned long x, unsigned long y)
{
    moveCamera(x, y);
    current_camera = desired_camera;
}

void Map::parsePlayerData(const Poco::DynamicAny& data)
{
    player_id = data["id"];
}

void Map::parseMapPositionsData(const Poco::DynamicAny& data)
{
    unsigned long x = data["PLAYER_X"];
    unsigned long y = data["PLAYER_Y"];
    setCamera(x, y);
    players[player_id].setTexture(ResourceManager::getTexture(player_looktype, Graphic::DIRECT));
    players[player_id].set_position(x, y);
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

void Map::addMap(const Poco::DynamicAny& data)
{
    map_data.emplace_back(ResourceManager::getTexture(data["id"], Graphic::MAP));
}

void Map::addMapData(const Poco::DynamicAny& data)
{
    addMapData(ResourceManager::getTexture(data["source"], Graphic::MAP_DATA),
               data["x"], data["y"]);
}

void Map::addMapData(const sf::Texture& texture, unsigned long x, unsigned long y)
{
    sf::Vector2u texture_size = texture.getSize();
    map_data.emplace_back(texture);
    map_data.back().setPosition(texture_size.x * x, texture_size.y * y);
}

void Map::addTile(const Poco::DynamicAny& data)
{
    if(data["type"] == 2)
    {
        doors.emplace_back(ResourceManager::getTexture(data["bg"], Graphic::DIRECT));
        doors.back().set_position(data["x"], data["y"]);
    }
}

void Map::addChest(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    if(data["open"])
        chests[id].setTexture(ResourceManager::getTexture(data["type"], Graphic::CHEST_OPEN));
    else
        chests[id].setTexture(ResourceManager::getTexture(data["type"], Graphic::CHEST));
    chests[id].set_position(data["x"], data["y"]);
}

void Map::addMonster(const Poco::DynamicAny& data)
{
    unsigned long id = data["id"];
    monsters[id].setTexture(ResourceManager::getTexture(data["looktype"], Graphic::MONSTER),
                            data["width"], data["height"]);
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

void Map::clear()
{
    sf::Lock lock(mutex);

    map_data.clear();
    doors.clear();
    chests.clear();
    map_items.clear();
    monsters.clear();
    npcs.clear();
    players.clear();
}
