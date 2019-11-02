#include "npc.hpp"

void NPC::set_texture(const sf::Texture& texture)
{
    rect.width = 32;
    rect.height = 48;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void NPC::set_position(int x, int y)
{
    //view-source:http://fantasy-world.pl/templates/client/default/js/map.js
    sprite.setPosition((32 * x) - 32, (32 * y) - 48);
}

void NPC::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
