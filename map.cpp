#include "map.hpp"
#include "resourcemanager.hpp"
#include "helperfunctions.hpp"
#include <SFML/System/Lock.hpp>

//view-source:http://fantasy-world.pl/templates/client/default/js/map.js

void Map::loadData(const Poco::DynamicStruct& data)
{
    clear();

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

    if(data.contains("player"))
        addMe(data["player"].extract<Poco::DynamicStruct>());
}

void Map::updateData(const Poco::DynamicStruct& data)
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
        }//end switch
    }//end for
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
    players[id].set_position(data["x"], data["y"]);
}

void Map::movePlayer(const Poco::DynamicStruct& data)
{
    int id = data["player"];
    players[id].set_dir(data["dir"]);
    players[id].move(data["x"], data["y"]);
}

void Map::moveMe(const Poco::DynamicStruct& data)
{
    players[id].set_dir(data["dir"]);
    players[id].move(data["x"], data["y"]);
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

void Map::draw(sf::RenderWindow& window)
{
    sf::Lock lock(mutex);

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
        doors.emplace_back();
        doors.back().setTexture(ResourceManager::getTexture(data["bg"]));
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

void Map::addMe(const Poco::DynamicStruct& data)
{
    id = data["id"];
    players[id].setTexture(ResourceManager::getTexture(ResourceManager::getLooktype()));
    players[id].set_position(data["x"], data["y"]);
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
