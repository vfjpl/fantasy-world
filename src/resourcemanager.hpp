#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <map>

enum class Graphic
{
	ATTACK_EFFECT,
	NPC,
	PLAYER,
	MONSTER,
	MAP_MULTI,
	MAP_SINGLE,
	ITEM,
	CHEST,
	CHEST_OPEN,
	GAME_OBJECT,
	DIRECT,
};

class ResourceManager_t
{
	//48
	std::map<std::string, sf::Texture> storage;

public:
	const sf::Texture& getTexture(const std::string& name, Graphic type);
};

extern ResourceManager_t ResourceManager;

#endif // RESOURCEMANAGER_HPP_INCLUDED
