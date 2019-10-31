#include "map.hpp"
#include <SFML/Graphics/Texture.hpp>

void Map::set_texture(const sf::Texture& texture, int x, int y)
{
    tiles.emplace_back(texture);
    sf::Vector2u texture_size = texture.getSize();
    tiles.back().setPosition(texture_size.x * x, texture_size.y * y);
}

void Map::draw(sf::RenderWindow& window)
{
    for(sf::Uint8 i = 0; i < tiles.size(); ++i)
        window.draw(tiles[i]);
}
