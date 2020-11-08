#include "npc.hpp"
#include "helperfunctions.hpp"
#include <SFML/Graphics/Texture.hpp>

void Npc::setTexture(const sf::Texture& texture)
{
    sf::Vector2u size = texture.getSize();
    frames_x = size.x/32;
    frames_y = size.y/48;
    rect.width = 32;
    rect.height = 48;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void Npc::set_dir(unsigned long dir)
{
    rect.left = (++steps % frames_x) * rect.width;
    rect.top = (dir % frames_y) * rect.height;
    sprite.setTextureRect(rect);
}

void Npc::set_position(unsigned long x, unsigned long y)
{
    move(x, y);
    current_px = desired_px;
}

void Npc::move(unsigned long x, unsigned long y)
{
    desired_px.x = (32 * x) - 32;
    desired_px.y = (32 * y) - 48;
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
