#include "monster.hpp"
#include "helperfunctions.hpp"
#include <SFML/Graphics/Texture.hpp>

void Monster::setTexture(const sf::Texture& texture, unsigned long width, unsigned long height)
{
    sf::Vector2u size = texture.getSize();
    frames_x = std::max(size.x/width, 1ul);
    frames_y = std::max(size.y/height, 1ul);
    rect.width = width;
    rect.height = height;
    sprite.setTextureRect(rect);
    sprite.setTexture(texture, false);
}

void Monster::setDir(unsigned long dir)
{
    rect.left = (++steps % frames_x) * rect.width;
    rect.top = (dir % frames_y) * rect.height;
    sprite.setTextureRect(rect);
}

void Monster::setPosition(unsigned long x, unsigned long y)
{
    move(x, y);
    current_px = desired_px;
}

void Monster::move(unsigned long x, unsigned long y)
{
    position_x = x;
    position_y = y;
    desired_px.x = (x * 32) - ((rect.width - 32)/2);
    desired_px.y = (y * 32) - (rect.height - 32);
}

bool Monster::isOnPosition(unsigned long x, unsigned long y)
{
    return (position_x == x && position_y == y);
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
