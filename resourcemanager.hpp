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
    //32
    static std::string player_looktype;

public:
    static const sf::Texture& getTexture(const std::string& name, Graphic type);
    static const std::string& getPlayerLooktype();
    static void setPlayerLooktype(const std::string& looktype);

private:
    static const sf::Texture& getTexture(const std::string& name);
    static void loadGraphic(const std::string& name);
};

#endif // RESOURCEMANAGER_HPP_INCLUDED
