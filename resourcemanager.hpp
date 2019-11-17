#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

#include <Poco/DynamicStruct.h>
#include <SFML/Graphics/Texture.hpp>

enum Graphic
{
    MAP,
    MAP_DATA,
    CHEST,
    CHEST_OPEN,
    ITEM,
    MONSTER,
    NPC,
    PLAYER,
};

class ResourceManager
{
    static std::map<std::string, sf::Texture> storage;

public:
    static const sf::Texture& getTexture(const std::string& name, Graphic type);
    static const sf::Texture& getTexture(const std::string& name);
    static void loadParallel(const Poco::DynamicStruct& data);
    static void loadParallel(const std::set<std::string>& names);

private:
    static void loadGraphic(const std::string& name);
};

#endif // RESOURCEMANAGER_HPP_INCLUDED
