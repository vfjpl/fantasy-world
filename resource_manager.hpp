#ifndef RESOURCE_MANAGER_HPP_INCLUDED
#define RESOURCE_MANAGER_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <set>

enum Graphic
{
    MAP_TILE,
    MAP_SINGLE,
    MONSTER,
    NPC,
    PLAYER,
};

class Resource_Manager
{
    std::map<std::string, sf::Texture> storage;

public:
    std::string getURI(const std::string& name, Graphic type);

    const sf::Texture& getTexture(const std::string& name, Graphic type);
    const sf::Texture& getTexture(const std::string& name);

    void loadGraphic(const std::string& name, Graphic type);
    void loadGraphic(const std::string& name);

    void loadParallel(const std::set<std::string>& names);
};

#endif // RESOURCE_MANAGER_HPP_INCLUDED
