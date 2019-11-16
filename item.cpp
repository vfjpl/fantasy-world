#include "item.hpp"

//view-source:http://fantasy-world.pl/templates/client/default/js/map.js

void Item::setTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture);
}

void Item::set_position(int x, int y)
{
    sprite.setPosition((x * 32) - 32, (y * 32) - 32);
}

void Item::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
