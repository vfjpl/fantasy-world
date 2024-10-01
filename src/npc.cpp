#include "npc.hpp"
#include "helperfunctions.hpp"
#include <SFML/Graphics/Texture.hpp>

// view-source:http://alkatria.pl/templates/client/default/js/npc.js

void Npc::setTexture(const sf::Texture& texture)
{
    sf::Vector2u size = texture.getSize();
    sf::Vector2u frames(size.x/32, size.y/48);
    frames_x = std::max<long>(frames.x, 1);
    frames_y = std::max<long>(frames.y, 1);
    rect.width = frames.x == 1 ? size.x : 32;
    rect.height = frames.y == 1 ? size.y : 48;
    sprite.setTextureRect(rect);
    sprite.setTexture(texture, false);
}

void Npc::setDir(long dir)
{
    rect.left = (++steps % frames_x) * 32;
    rect.top = (dir % frames_y) * 48;
    sprite.setTextureRect(rect);
}

void Npc::setPosition(long x, long y, bool can_walk)
{
    npc_can_walk = can_walk;
    move(x, y);
    current_px = desired_px;
}

void Npc::move(long x, long y)
{
    position_x = x;
    position_y = y;
    desired_px.x = (x * 32) - 32;
    desired_px.y = (y * 32) - 48;
}

bool Npc::isOnPosition(long x, long y)
{
    return (position_x == x && position_y == y && !npc_can_walk);
}

bool Npc::contains(sf::Vector2f coords)
{
    return sprite.getGlobalBounds().contains(coords);
}

void Npc::draw(sf::RenderWindow& window)
{
    current_px += clamp(desired_px - current_px);
    sprite.setPosition(current_px.x, current_px.y);
    window.draw(sprite);
}
