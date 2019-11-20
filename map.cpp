#include "map.hpp"
#include "resourcemanager.hpp"
#include "playerdata.hpp"
#include "helperfunctions.hpp"
#include <SFML/System/Lock.hpp>
#include <iostream>

//view-source:http://fantasy-world.pl/templates/client/default/js/map.js

void Map::initDefaultCamera(const sf::View& view)
{
    camera = view;
}

void Map::loadMapData(const Poco::DynamicStruct& data)
{
    ResourceManager::loadParallel(data);
    clear();

    if(data.contains("player"))
        loadPlayerData(data["player"].extract<Poco::DynamicStruct>());
    loadMapPositions(data["map_positions"].extract<Poco::DynamicStruct>());

    if(data.contains("map_data"))
    {
        auto& map_data = data["map_data"];
        for(sf::Uint8 i = 0; i < map_data.size(); ++i)
            addMapData(map_data[i].extract<Poco::DynamicStruct>());
    }
    else
    {
        addMap(data["map"].extract<Poco::DynamicStruct>());
    }

    auto& tiles = data["tiles"];
    for(sf::Uint8 i = 0; i < tiles.size(); ++i)
        addTile(tiles[i].extract<Poco::DynamicStruct>());

    auto& chests = data["chests"];
    for(sf::Uint8 i = 0; i < chests.size(); ++i)
        addChest(chests[i].extract<Poco::DynamicStruct>());

    if(data.contains("map_items"))
    {
        auto& map_items = data["map_items"];
        for(sf::Uint8 i = 0; i < map_items.size(); ++i)
            addMapItem(map_items[i].extract<Poco::DynamicStruct>());
    }

    auto& monsters = data["monsters"];
    for(sf::Uint8 i = 0; i < monsters.size(); ++i)
        addMonster(monsters[i].extract<Poco::DynamicStruct>());

    auto& npcs = data["npcs"];
    for(sf::Uint8 i = 0; i < npcs.size(); ++i)
        addNpc(npcs[i].extract<Poco::DynamicStruct>());

    auto& players = data["players"];
    for(sf::Uint8 i = 0; i < players.size(); ++i)
        addPlayer(players[i].extract<Poco::DynamicStruct>());
}

void Map::updateMapData(const Poco::DynamicStruct& data)
{
    for(auto &i: data)
    {
        switch(str2int(i.first))
        {
        case char2int("moves"):
        {
            auto& monsters = i.second;
            for(sf::Uint8 i = 0; i < monsters.size(); ++i)
                moveMonster(monsters[i].extract<Poco::DynamicStruct>());
            break;
        }
        case char2int("npc_moves"):
        {
            auto& npcs = i.second;
            for(sf::Uint8 i = 0; i < npcs.size(); ++i)
                moveNpc(npcs[i].extract<Poco::DynamicStruct>());
            break;
        }
        case char2int("respawns"):
        {
            auto& monsters = i.second;
            for(sf::Uint8 i = 0; i < monsters.size(); ++i)
                addMonster(monsters[i].extract<Poco::DynamicStruct>());
            break;
        }
        case char2int("yells"):
        {
            break;
        }
        case char2int("damages"):
        {
            break;
        }
        default:
        {
            std::cout << i.first << '\n';
            break;
        }
        }//end switch
    }//end for
}

void Map::movePlayer(const Poco::DynamicStruct& data)
{
    int id = data["player"];
    players[id].set_dir(data["dir"]);
    players[id].move(data["x"], data["y"]);
}

void Map::moveMe(const Poco::DynamicStruct& data)
{
    PlayerData::position.x = data["x"];
    PlayerData::position.y = data["y"];
    moveCamera(PlayerData::position);
    players[PlayerData::id].set_dir(data["dir"]);
    players[PlayerData::id].move(PlayerData::position);
}

void Map::addMapItem(const Poco::DynamicStruct& data)
{
    int id = data["id"];
    map_items[id].setTexture(ResourceManager::getTexture(data["item_id"], ITEM));
    map_items[id].set_position(data["x"], data["y"]);
}

void Map::addPlayer(const Poco::DynamicStruct& data)
{
    int id = data["id"];
    players[id].setTexture(ResourceManager::getTexture(data["looktype"], PLAYER));
    players[id].set_position_server_bug(data["x"], data["y"]);
}

void Map::deleteMapItem(const Poco::DynamicStruct& data)
{
    int id = data["id"];
    map_items.erase(id);
}

void Map::deleteMonster(const Poco::DynamicStruct& data)
{
    int id = data["id"];
    monsters.erase(id);
}

void Map::deletePlayer(const Poco::DynamicStruct& data)
{
    int id = data["id"];
    players.erase(id);
}

int Map::getCloseMonsterId()
{
    for(auto &i : monsters)
        if(inRange(i.second.getPosition() - PlayerData::position))
            return i.first;
    return 0;
}

void Map::draw(sf::RenderWindow& window)
{
    sf::Lock lock(mutex);

    current_camera += clamp(desired_camera - current_camera);
    camera.setCenter(current_camera.x, current_camera.y);
    window.setView(camera);

    for(auto &i: map_data)
        window.draw(i);
    for(auto &i: doors)
        i.draw(window);
    for(auto &i: chests)
        i.second.draw(window);
    for(auto &i: map_items)
        i.second.draw(window);
    for(auto &i: monsters)
        i.second.draw(window);
    for(auto &i: npcs)
        i.second.draw(window);
    for(auto &i: players)
        i.second.draw(window);
}

// private

void Map::loadPlayerData(const Poco::DynamicStruct& data)
{
    PlayerData::id = data["id"];
}

void Map::loadMapPositions(const Poco::DynamicStruct& data)
{
    PlayerData::position.x = data["PLAYER_X"];
    PlayerData::position.y = data["PLAYER_Y"];
    setCamera(PlayerData::position);
    players[PlayerData::id].setTexture(ResourceManager::getTexture(PlayerData::looktype));
    players[PlayerData::id].set_position(PlayerData::position);
}

void Map::moveCamera(sf::Vector2i pos)
{
    desired_camera.x = (32 * pos.x) - 16;
    desired_camera.y = (32 * pos.y) - 16;
}

void Map::setCamera(sf::Vector2i pos)
{
    moveCamera(pos);
    current_camera = desired_camera;
}

void Map::moveMonster(const Poco::DynamicStruct& data)
{
    int id = data["monster"];
    monsters[id].set_dir(data["dir"]);
    monsters[id].move(data["x"], data["y"]);
}

void Map::moveNpc(const Poco::DynamicStruct& data)
{
    int id = data["npc"];
    npcs[id].set_dir(data["dir"]);
    npcs[id].move(data["x"], data["y"]);
}

void Map::addMap(const Poco::DynamicStruct& data)
{
    map_data.emplace_back(ResourceManager::getTexture(data["id"], MAP));
}

void Map::addMapData(const Poco::DynamicStruct& data)
{
    addMapData(ResourceManager::getTexture(data["source"], MAP_DATA), data["x"], data["y"]);
}

void Map::addMapData(const sf::Texture& texture, int x, int y)
{
    sf::Vector2u texture_size = texture.getSize();
    map_data.emplace_back(texture);
    map_data.back().setPosition(texture_size.x * x, texture_size.y * y);
}

void Map::addTile(const Poco::DynamicStruct& data)
{
    if(data["type"] == 2)
    {
        doors.emplace_back(ResourceManager::getTexture(data["bg"]));
        doors.back().set_position(data["x"], data["y"]);
    }
}

void Map::addChest(const Poco::DynamicStruct& data)
{
    int id = data["id"];
    if(data["open"])
        chests[id].setTexture(ResourceManager::getTexture(data["type"], CHEST_OPEN));
    else
        chests[id].setTexture(ResourceManager::getTexture(data["type"], CHEST));
    chests[id].set_position(data["x"], data["y"]);
}

void Map::addMonster(const Poco::DynamicStruct& data)
{
    int id = data["id"];
    monsters[id].setTexture(ResourceManager::getTexture(data["looktype"], MONSTER), data["width"], data["height"]);
    monsters[id].set_position(data["x"], data["y"]);
}

void Map::addNpc(const Poco::DynamicStruct& data)
{
    int id = data["id"];
    npcs[id].setTexture(ResourceManager::getTexture(data["looktype"], NPC));
    npcs[id].set_position(data["x"], data["y"]);
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
