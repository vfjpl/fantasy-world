#include "npc.hpp"
#include "helperfunctions.hpp"
#include <SFML/Graphics/Texture.hpp>

void Npc::setTexture(const sf::Texture& texture)
{
    sf::Vector2u size = texture.getSize();
    frames_x = std::max(size.x/32, 1u);
    frames_y = std::max(size.y/48, 1u);
    rect.width = 32;
    rect.height = 48;
    sprite.setTextureRect(rect);
    sprite.setTexture(texture, false);
}

void Npc::setDir(unsigned long dir)
{
    rect.left = (++steps % frames_x) * 32;
    rect.top = (dir % frames_y) * 48;
    sprite.setTextureRect(rect);
}

void Npc::setPosition(unsigned long x, unsigned long y, bool can_walk)
{
    npc_can_walk = can_walk;
    move(x, y);
    current_px = desired_px;
}

void Npc::move(unsigned long x, unsigned long y)
{
    position_x = x;
    position_y = y;
    desired_px.x = (x * 32) - 32;
    desired_px.y = (y * 32) - 48;
}

bool Npc::isOnPosition(unsigned long x, unsigned long y)
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
