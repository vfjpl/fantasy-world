#ifndef RESOURCE_MANAGER_HPP_INCLUDED
#define RESOURCE_MANAGER_HPP_INCLUDED

#include <Poco/DynamicStruct.h>
#include <SFML/Graphics/Texture.hpp>

enum Graphic
{
    MAP_TILE,
    MAP_SINGLE,
    CHEST_CLOSED,
    CHEST_OPENED,
    ITEM,
    MONSTER,
    NPC,
    PLAYER,
};

class Resource_Manager
{
    std::map<std::string, sf::Texture> storage;

public:
    const sf::Texture& getTexture(const std::string& name, Graphic type);
    const sf::Texture& getTexture(const std::string& name);
    void loadParallel(const Poco::DynamicStruct& data);
    void loadParallel(const std::set<std::string>& names);

private:
    void loadGraphic(const std::string& name);
};

#endif // RESOURCE_MANAGER_HPP_INCLUDED
