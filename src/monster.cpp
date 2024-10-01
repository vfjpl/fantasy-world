#include "monster.hpp"
#include "helperfunctions.hpp"
#include <SFML/Graphics/Texture.hpp>

void Monster::setTexture(const sf::Texture& texture, long width, long height)
{
    const sf::Vector2u size = texture.getSize();
    frames_x = std::max<long>(size.x/width, 1);
    frames_y = std::max<long>(size.y/height, 1);
    rect.width = width;
    rect.height = height;
    sprite.setTextureRect(rect);
    sprite.setTexture(texture, false);
}

void Monster::setDir(long dir)
{
    rect.left = (++steps % frames_x) * rect.width;
    rect.top = (dir % frames_y) * rect.height;
    sprite.setTextureRect(rect);
}

void Monster::setPosition(long x, long y)
{
    move(x, y);
    current_px = desired_px;
}

void Monster::move(long x, long y)
{
    position_x = x;
    position_y = y;
    desired_px.x = (x * 32) - ((rect.width - 32)/2);
    desired_px.y = (y * 32) - (rect.height - 32);
}

bool Monster::isOnPosition(long x, long y)
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
