#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

#include <Poco/DynamicStruct.h>
#include <SFML/Graphics/Texture.hpp>

enum Graphic
{
    MAP_DATA,
    MAP,
    CHEST_OPEN,
    CHEST,
    ITEM,
    MONSTER,
    NPC,
    PLAYER,
};

class ResourceManager
{
    //48
    static std::map<std::string, sf::Texture> storage;

public:
    //32
    static std::string playerLooktype;
    //4
    static int playerId;

    static const sf::Texture& getTexture(const std::string& name, Graphic type);
    static const sf::Texture& getTexture(const std::string& name);
    static void loadParallel(const Poco::DynamicStruct& data);

private:
    static void loadParallel(const std::set<std::string>& names);
    static void loadGraphic(const std::string& name);
};

#endif // RESOURCEMANAGER_HPP_INCLUDED
