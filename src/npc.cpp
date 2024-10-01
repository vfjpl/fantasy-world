#include "npc.hpp"
#include "helperfunctions.hpp"
#include <SFML/Graphics/Texture.hpp>

// view-source:http://alkatria.pl/templates/client/default/js/npc.js

void Npc::setTexture(const sf::Texture& texture)
{
    const sf::Vector2u size = texture.getSize();
    const long calc_frames_x = size.x/32l;
    const long calc_frames_y = size.y/48l;
    frames_x = std::max<long>(calc_frames_x, 1l);
    frames_y = std::max<long>(calc_frames_y, 1l);
    rect.width = calc_frames_x == 1l ? size.x : 32l;
    rect.height = calc_frames_y == 1l ? size.y : 48l;
    sprite.setTextureRect(rect);
    sprite.setTexture(texture, false);
}

void Npc::setDir(long dir)
{
    rect.left = (++steps % frames_x) * 32l;
    rect.top = (dir % frames_y) * 48l;
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
    desired_px.x = (x * 32l) - 32l;
    desired_px.y = (y * 32l) - 48l;
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
