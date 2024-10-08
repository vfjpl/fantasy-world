#include "map.hpp"
#include "resourcemanager.hpp"
#include "localplayer.hpp"
#include "helperfunctions.hpp"
#include <Poco/DynamicStruct.h>
#include <SFML/System/Lock.hpp>
#include <iostream>

// view-source:http://alkatria.pl/templates/client/default/js/map.js

Map_t Map;


void Map_t::moveCamera(long x, long y)
{
    desired_camera.x = (x * 32l) - 16l;
    desired_camera.y = (y * 32l);
}

void Map_t::setCamera(long x, long y)
{
    moveCamera(x, y);
    current_camera = desired_camera;
}

bool Map_t::isMapItem(sf::Vector2f coords)
{
    for(auto& i : map_items)
        if(i.second.contains(coords))
            return true;
    return false;
}

bool Map_t::isTile(sf::Vector2f coords)
{
    for(auto& i : tiles)
        if(i.contains(coords))
            return true;
    return false;
}

long Map_t::getPlayerIDf(sf::Vector2f coords)
{
    for(auto& i : players)
        if(i.second.contains(coords))
            return i.first;
    return 0l;
}

long Map_t::getNpcIDf(sf::Vector2f coords)
{
    for(auto& i : npcs)
        if(i.second.contains(coords))
            return i.first;
    return 0l;
}

long Map_t::getMonsterIDf(sf::Vector2f coords)
{
    for(auto& i : monsters)
        if(i.second.contains(coords))
            return i.first;
    return 0l;
}

long Map_t::getChestIDf(sf::Vector2f coords)
{
    for(auto& i : chests)
        if(i.second.contains(coords))
            return i.first;
    return 0l;
}

void Map_t::moveNpc(const Poco::DynamicAny& data)
{
    long id = data["npc"];
    npcs[id].move(data["x"], data["y"]);
    npcs[id].setDir(data["dir"]);
}

void Map_t::moveMonster(const Poco::DynamicAny& data)
{
    long id = data["monster"];
    monsters[id].move(data["x"], data["y"]);
    monsters[id].setDir(data["dir"]);
}

void Map_t::moveComrade(const Poco::DynamicAny& data)
{
	Comrade& comrade = comrades[toLong(data["comrade"])];
	comrade.move(toLong(data["x"]), toLong(data["y"]));
	comrade.setDir(toLong(data["dir"]));
}

void Map_t::addNpc(const Poco::DynamicAny& data)
{
    long id = data["id"];
    npcs[id].setTexture(ResourceManager.getTexture(data["looktype"], Graphic::NPC));
    npcs[id].setPosition(data["x"], data["y"], data["can_walk"]);
}

void Map_t::addMonster(const Poco::DynamicAny& data)
{
    long id = data["id"];
    monsters[id].setTexture(ResourceManager.getTexture(data["looktype"], Graphic::MONSTER), data["width"], data["height"]);
    monsters[id].setPosition(data["x"], data["y"]);
}

void Map_t::addChest(const Poco::DynamicAny& data)
{
    long id = data["id"];
    chests[id].setTexture(ResourceManager.getTexture(data["type"], data["open"] ? Graphic::CHEST_OPEN : Graphic::CHEST));
    chests[id].setPosition(data["x"], data["y"]);
}

void Map_t::addMapObject(const Poco::DynamicAny& data)
{
    long id = data["id"];
    map_objects[id].setTexture(ResourceManager.getTexture(data["file"], Graphic::GAME_OBJECT), data["width"], data["height"]);
    map_objects[id].setPosition(data["x"], data["y"]);
}

void Map_t::addTile(const Poco::DynamicAny& data)
{
    switch(toLong(data["type"]))
    {
    case 2://door
    {
        const sf::Texture& texture = ResourceManager.getTexture(data["bg"], Graphic::DIRECT);
        long x = data["x"];
        long y = data["y"];
        sf::Lock lock(mutex);
        tiles.emplace_back(texture);
        tiles.back().setPosition(x, y);
        break;
    }
    case 21://well
    {
        long x = data["x"];
        long y = data["y"];
        sf::Lock lock(mutex);
        tiles.emplace_back();
        tiles.back().setPosition(x, y);
        break;
    }
    case 120://sign
    {
        const auto& image = data["data"]["image"];
        const sf::Texture& texture = ResourceManager.getTexture(image["path"], Graphic::DIRECT);
        long width = image["width"];
        long height = image["height"];
        long x = data["x"];
        long y = data["y"];
        //todo
        break;
    }
    default:
    {
        const sf::Texture& texture = ResourceManager.getTexture(data["tile"], Graphic::GAME_OBJECT);
        long width = data["width"];
        long height = data["height"];
        long x = data["x"];
        long y = data["y"];
        sf::Lock lock(mutex);
        tiles.emplace_back(texture, width, height);
        tiles.back().setPosition(x, y);
        break;
    }
    }//end switch
}

void Map_t::addMultiMapData(const Poco::DynamicAny& data)
{
    const sf::Texture& texture = ResourceManager.getTexture(data["source"], Graphic::MAP_MULTI);
    long x = data["x"] * 640l;
    long y = data["y"] * 640l;
    sf::Lock lock(mutex);
    map_backgrounds.emplace_back(texture);
    map_backgrounds.back().setPosition(x, y);
}

void Map_t::addSingleMapData(const Poco::DynamicAny& data)
{
    const sf::Texture& texture = ResourceManager.getTexture(data["id"], Graphic::MAP_SINGLE);
    sf::Lock lock(mutex);
    map_backgrounds.emplace_back(texture);
}

void Map_t::loadMapPositionData(const Poco::DynamicAny& data)
{
    max_x = data["MAX_X"];
    max_y = data["MAX_Y"];
    LocalPlayer.x = data["PLAYER_X"];
    LocalPlayer.y = data["PLAYER_Y"];
    setCamera(LocalPlayer.x, LocalPlayer.y);
    players[LocalPlayer.id].setTexture(ResourceManager.getTexture(LocalPlayer.looktype, Graphic::DIRECT));
    players[LocalPlayer.id].setPosition(LocalPlayer.x, LocalPlayer.y);
}


void Map_t::updateWindowSize(float width, float height)
{
    camera.setSize(width, height);
}

void Map_t::moveLocalPlayer(const Poco::DynamicAny& data)
{
    LocalPlayer.x = data["x"];
    LocalPlayer.y = data["y"];
    moveCamera(LocalPlayer.x, LocalPlayer.y);
    players[LocalPlayer.id].move(LocalPlayer.x, LocalPlayer.y);
    players[LocalPlayer.id].setDir(data["dir"]);
}

void Map_t::loadData_100(const Poco::DynamicAny& data)
{
    LocalPlayer.id = data["player"]["id"];
    loadData_teleport(data);
}

void Map_t::loadData_teleport(const Poco::DynamicAny& data)
{
	obstacles.swap((Poco::DynamicAny&)data["obstacles"]);
	loadMapPositionData(data["map_positions"]);

	//todo better
	if(const auto& map_data = data["map"]; map_data["type"] == 2l)
	{
		addSingleMapData(map_data);
	}
	else
	{
		for(const auto& map_chunk : data["map_data"])
			addMultiMapData(map_chunk);
	}

	if(const auto& tiles_data = data["tiles"]; tiles_data.size())
		for(const auto& tile : tiles_data)
			addTile(tile);
	if(const auto& map_objects_data = data["map_objects"]; map_objects_data.size())
		for(const auto& map_object : map_objects_data)
			addMapObject(map_object);
	if(const auto& chests_data = data["chests"]; chests_data.size())
		for(const auto& chest : chests_data)
			addChest(chest);
	if(const auto& map_items_data = data["map_items"]; map_items_data.size())
		for(const auto& map_item : map_items_data)
			addMapItem(map_item);
	if(const auto& monsters_data = data["monsters"]; monsters_data.size())
		for(const auto& monster : monsters_data)
			addMonster(monster);
	if(const auto& npcs_data = data["npcs"]; npcs_data.size())
		for(const auto& npc : npcs_data)
			addNpc(npc);
	if(const auto& comrades_data = data["comraded"]; comrades_data.size())
		for(const auto& comrade : comrades_data)
			addComrade(comrade);
	if(const auto& players_data = data["players"]; players_data.size())
		for(const auto& player : players_data)
			addPlayer(player);
}

void Map_t::updateMapData(const Poco::DynamicAny& data)
{
	if(!data.size())//can be empty, just {"code":55}
		return;

	for(const auto& i : data.extract<Poco::DynamicStruct>())
	{
		switch(hashString(i.first))
		{
		case hashCharPtr("moves"):
			for(const auto& monster : i.second)
				moveMonster(monster);
			break;
		case hashCharPtr("npc_moves"):
			for(const auto& npc : i.second)
				moveNpc(npc);
			break;
		case hashCharPtr("comrade_moves"):
			for(const auto& comrade : i.second)
				moveComrade(comrade);
			break;
		case hashCharPtr("respawns"):
			for(const auto& monster : i.second)
				addMonster(monster);
			break;
		case hashCharPtr("damages"):
			break;
		case hashCharPtr("spells"):
			break;
		case hashCharPtr("yells"):
			break;
		case hashCharPtr("monster_yells"):
			break;
		default:
			std::cout << "updateMapData: " << toString(data) << '\n';
			break;
		}//end switch
	}//end for
}

void Map_t::moveOutfit(const Poco::DynamicAny& data)
{
    long id = data["player"];
    players[id].setDir(data["dir"]);
}

void Map_t::movePlayer(const Poco::DynamicAny& data)
{
    long id = data["player"];
    players[id].move(data["x"], data["y"]);
    players[id].setDir(data["dir"]);
}

void Map_t::addMapItem(const Poco::DynamicAny& data)
{
    long id = data["id"];
    map_items[id].setTexture(ResourceManager.getTexture(data["item_id"], Graphic::ITEM));
    map_items[id].setPosition(data["x"], data["y"]);
}

void Map_t::addComrade(const Poco::DynamicAny& data)
{
	Comrade& comrade = comrades[toLong(data["id_player"])];//we use player id for comrade?
	comrade.setTexture(ResourceManager.getTexture(toString(data["path"]), Graphic::DIRECT), toLong(data["width"]), toLong(data["height"]));
	comrade.setPosition(toLong(data["x"]), toLong(data["y"]));
}

void Map_t::addPlayer(const Poco::DynamicAny& data)
{
    long id = data["id"];
    players[id].setTexture(ResourceManager.getTexture(data["looktype"], Graphic::PLAYER));
    players[id].setPosition(data["x"] + 1l, data["y"] + 1l);//server bug
}

void Map_t::deleteMapItem(const Poco::DynamicAny& data)
{
    long id = data["id"];
    map_items.erase(id);
}

void Map_t::deleteMonster(const Poco::DynamicAny& data)
{
    long id = data["id"];
    monsters.erase(id);
}

void Map_t::deletePlayer(const Poco::DynamicAny& data)
{
	long id = toLong(data["id"]);
	players.erase(id);
	comrades.erase(id);
}

void Map_t::openChest(const Poco::DynamicAny& data)
{
    //todo better
    if(const auto& chest_data = data["chest"]; chest_data.size())
    {
        long id = chest_data;
        chests[id].setTexture(ResourceManager.getTexture(std::to_string(1l), Graphic::CHEST_OPEN));
    }
}

void Map_t::updateTile(const Poco::DynamicAny& data)
{
    long x = data["x"];
    long y = data["y"];
    //todo better
    for(auto& i : tiles)
    {
        if(i.isOnPosition(x, y))
        {
            i.setTexture(ResourceManager.getTexture(data["file"], Graphic::GAME_OBJECT));
            break;
        }
    }
}

MapClickData Map_t::mapMouseClick(sf::RenderWindow& window, sf::Vector2i point)
{
    sf::Vector2f coords = window.mapPixelToCoords(point, camera);
    return {getChestIDf(coords), getMonsterIDf(coords), getNpcIDf(coords), getPlayerIDf(coords),
            (long)std::ceil(coords.x/32.f), (long)std::ceil(coords.y/32.f),
            isTile(coords), isMapItem(coords)};
}

long Map_t::getMonsterID(long x, long y)
{
    for(auto& i : monsters)
        if(i.second.isOnPosition(x, y))
            return i.first;
    return 0l;
}

bool Map_t::isObstacle(long x, long y)
{
	if((x < 0l) | (x >= max_x) | (y < 0l) | (y >= max_y))
		return true;
	return obstacles[x][y];
}

bool Map_t::isNpc(long x, long y)
{
    for(auto& i : npcs)
        if(i.second.isOnPosition(x, y))
            return true;
    return false;
}

void Map_t::draw(sf::RenderWindow& window)
{
	sf::Lock lock(mutex);

	current_camera += clamp(desired_camera - current_camera);
	camera.setCenter(current_camera.x, current_camera.y);
	window.setView(camera);

	for(auto& i : map_backgrounds)
		window.draw(i);
	for(auto& i : tiles)
		i.draw(window);
	for(auto& i : map_objects)
		i.second.draw(window);
	for(auto& i : chests)
		i.second.draw(window);
	for(auto& i : map_items)
		i.second.draw(window);
	for(auto& i : monsters)
		i.second.draw(window);
	for(auto& i : npcs)
		i.second.draw(window);
	for(auto& i : comrades)
		i.second.draw(window);
	for(auto& i : players)
		i.second.draw(window);
}

void Map_t::clear()
{
	sf::Lock lock(mutex);

	map_backgrounds.clear();
	tiles.clear();
	map_objects.clear();
	chests.clear();
	map_items.clear();
	monsters.clear();
	npcs.clear();
	comrades.clear();
	players.clear();
}
