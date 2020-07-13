#include "monster.hpp"
#include "helperfunctions.hpp"
#include <SFML/Graphics/Texture.hpp>

void Monster::setTexture(const sf::Texture& texture, unsigned long width, unsigned long height)
{
    frames = texture.getSize().x/width;
    rect.width = width;
    rect.height = height;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void Monster::set_dir(unsigned long dir)
{
    ++steps;
    rect.left = (steps%frames) * rect.width;
    rect.top = (dir%4) * rect.height;
    sprite.setTextureRect(rect);
}

void Monster::move(unsigned long x, unsigned long y)
{
    desired_px.x = (32 * x) - ((rect.width - 32)/2);
    desired_px.y = (32 * y) - (rect.height - 32);
}

void Monster::set_position(unsigned long x, unsigned long y)
{
    move(x, y);
    current_px = desired_px;
}

bool Monster::contains(sf::Vector2f coords)
{
    return sprite.getGlobalBounds().contains(coords);
}

void Monster::draw(sf::RenderWindow& window)
{
    current_px += clamp(desired_px - current_px);
    sprite.setPosition(current_px.x, current_px.y);
    window.draw(sprite);
}
