#include "npc.hpp"

//view-source:http://fantasy-world.pl/templates/client/default/js/map.js

void Npc::setTexture(const sf::Texture& texture)
{
    rect.width = 32;
    rect.height = 48;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void Npc::set_dir(int dir)
{
    rect.top = dir%4 * rect.height;
    sprite.setTextureRect(rect);
}

void Npc::set_position(int x, int y)
{
    move(x, y);
    sprite.setPosition(position.x, position.y);
}

void Npc::move(int x, int y)
{
    position.x = (32 * x) - 32;
    position.y = (32 * y) - 48;
}

void Npc::draw(sf::RenderWindow& window)
{
    sf::Vector2i diff = position - sf::Vector2i(sprite.getPosition());
    sprite.move(std::max(-1, std::min(diff.x, 1)), std::max(-1, std::min(diff.y, 1)));
    window.draw(sprite);
}
