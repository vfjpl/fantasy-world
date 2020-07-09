#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <map>

enum class Graphic
{
    MAP_DATA,
    MAP,
    CHEST_OPEN,
    CHEST,
    ITEM,
    MONSTER,
    NPC,
    PLAYER,
    DIRECT,
};

class ResourceManager
{
    //48
    static std::map<std::string, sf::Texture> storage;

public:
    static const sf::Texture& getTexture(const std::string& name, Graphic type);

private:
    static const sf::Texture& getTexture(const std::string& name);
    static void loadGraphic(const std::string& name);
};

#endif // RESOURCEMANAGER_HPP_INCLUDED
