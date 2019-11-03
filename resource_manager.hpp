#ifndef RESOURCE_MANAGER_HPP_INCLUDED
#define RESOURCE_MANAGER_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <map>

enum Graphic
{
    MAP_TILE,
    MAP_SINGLE,
    MONSTER,
    NPC,
    PLAYER,
    DIRECT,
};

class Resource_Manager
{
    std::map<std::string, sf::Texture> storage;
    unsigned long size_in_bytes = 0;

public:
    const sf::Texture& getTexture(const std::string& name, Graphic type);
    void loadGraphic(const std::string& name, Graphic type);
};

#endif // RESOURCE_MANAGER_HPP_INCLUDED
