#include "map.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Lock.hpp>

void Map::set_texture(const sf::Texture& texture, int x, int y)
{
    sf::Vector2u texture_size = texture.getSize();
    tiles.emplace_back(texture);
    tiles.back().setPosition(texture_size.x * x, texture_size.y * y);
}

void Map::clear()
{
    sf::Lock lock(mutex);

    tiles.clear();
    monsters.clear();
    NPCs.clear();
}

void Map::draw(sf::RenderWindow& window)
{
    sf::Lock lock(mutex);

    for(sf::Uint8 i = 0; i < tiles.size(); ++i)
        window.draw(tiles[i]);
    for(auto &i: monsters)
        i.second.draw(window);
    for(auto &i: NPCs)
        i.second.draw(window);
    for(auto &i: players)
        i.second.draw(window);
    player.draw(window);
}
