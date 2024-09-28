#include "resourcemanager.hpp"
#include "network.hpp"
#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <SFML/System/FileInputStream.hpp>

ResourceManager_t ResourceManager;


static std::string getPath(const std::string& name, Graphic type)
{
	switch(type)
	{
	case Graphic::ATTACK_EFFECT:
		return "cache/assets/attacks/" + name + ".gif";
	case Graphic::NPC:
		return "cache/assets/avatars/" + name + ".gif";
	case Graphic::PLAYER:
		return "cache/assets/looktypes/" + name + ".gif";
	case Graphic::MONSTER:
		return "cache/templates/client/default/images/monsters/" + name + ".gif";
	case Graphic::MAP_MULTI:
		return "cache/assets/" + name + ".png";
	case Graphic::MAP_SINGLE:
		return "cache/assets/maps/files/" + name + ".png";
	case Graphic::ITEM:
		return "cache/templates/client/default/images/items/" + name + ".png";
	case Graphic::CHEST:
		return "cache/templates/client/default/images/tiles/chest_" + name + ".png";
	case Graphic::CHEST_OPEN:
		return "cache/templates/client/default/images/tiles/chest_" + name + "_open.png";
	case Graphic::GAME_OBJECT:
		return "cache/assets/game_objects/" + name;
	default:
		return "cache" + name;
	}//end switch
}


const sf::Texture& ResourceManager_t::getTexture(const std::string& name, Graphic type)
{
	const std::string path = getPath(name, type);
	const auto loaded = storage.count(path);
	sf::Texture& texture = storage[path];
	if(!loaded)
	{
		sf::FileInputStream sfFile;
		if(!sfFile.open(path))
		{
			Poco::File(path.substr(0ul, path.rfind('/'))).createDirectories();
			Poco::FileOutputStream(path) << Network::receiveData(path.substr(path.find('/'))).rdbuf();
			sfFile.open(path);
		}
		texture.loadFromStream(sfFile);
	}
	return texture;
}
