#include "map.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Lock.hpp>

void Map::setTexture(const sf::Texture& texture, int x, int y)
{
    sf::Vector2u texture_size = texture.getSize();
    tiles.emplace_back(texture);
    tiles.back().setPosition(texture_size.x * x, texture_size.y * y);
}

void Map::clear()
{
    sf::Lock lock(mutex);

    tiles.clear();
    items.clear();
    monsters.clear();
    npcs.clear();
    players.clear();
}

void Map::draw(sf::RenderWindow& window)
{
    sf::Lock lock(mutex);

    for(auto &i: tiles)
        window.draw(i);
    for(auto &i: items)
        i.second.draw(window);
    for(auto &i: monsters)
        i.second.draw(window);
    for(auto &i: npcs)
        i.second.draw(window);
    for(auto &i: players)
        i.second.draw(window);
    player.draw(window);
}
