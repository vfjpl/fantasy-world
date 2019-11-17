#include "chest.hpp"

//view-source:http://fantasy-world.pl/templates/client/default/js/map.js

void Chest::setTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture);
}

void Chest::set_position(int x, int y)
{
    sprite.setPosition((32 * x) - 32, (32 * y) - 32);
}

void Chest::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
