#include "npc.hpp"
#include "helperfunctions.hpp"
#include <SFML/Graphics/Texture.hpp>

void Npc::setTexture(const sf::Texture& texture)
{
    anim = texture.getSize().x/32;
    rect.width = 32;
    rect.height = 48;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void Npc::set_dir(int dir)
{
    rect.top = (dir%4) * rect.height;
    rect.left = ((++steps)%anim) * rect.width;
    sprite.setTextureRect(rect);
}

void Npc::move(int x, int y)
{
    desired_px.x = (32 * x) - 32;
    desired_px.y = (32 * y) - 48;
}

void Npc::set_position(int x, int y)
{
    move(x, y);
    current_px = desired_px;
}

void Npc::draw(sf::RenderWindow& window)
{
    current_px += clamp(desired_px - current_px);
    sprite.setPosition(current_px.x, current_px.y);
    window.draw(sprite);
}
