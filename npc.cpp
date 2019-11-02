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
    position.x = (32 * x) - 32;
    position.y = (32 * y) - 48;
    sprite.setPosition(position.x, position.y);
}

void NPC::move(int x, int y)
{
    position.x = (32 * x) - 32;
    position.y = (32 * y) - 48;
}

void NPC::draw(sf::RenderWindow& window)
{
    sf::Vector2i diff = position - sf::Vector2i(sprite.getPosition());
    sprite.move(std::max(-1, std::min(diff.x, 1)), std::max(-1, std::min(diff.y, 1)));
    window.draw(sprite);
}
