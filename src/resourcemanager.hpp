#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>

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

namespace ResourceManager
{
const sf::Texture& getTexture(const std::string& name, Graphic type);
}

#endif // RESOURCEMANAGER_HPP_INCLUDED
