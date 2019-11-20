#include "monster.hpp"
#include "helperfunctions.hpp"

void Monster::setTexture(const sf::Texture& texture, int width, int height)
{
    rect.width = width;
    rect.height = height;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void Monster::set_dir(int dir)
{
    rect.top = (dir%4) * rect.height;
    sprite.setTextureRect(rect);
}

void Monster::move(int x, int y)
{
    position.x = x;
    position.y = y;
    desired_px.x = (32 * x) - ((rect.width - 32)/2);
    desired_px.y = (32 * y) - (rect.height - 32);
}

void Monster::set_position(int x, int y)
{
    move(x, y);
    current_px = desired_px;
}

sf::Vector2i Monster::getPosition()
{
    return position;
}

void Monster::draw(sf::RenderWindow& window)
{
    current_px += clamp(desired_px - current_px);
    sprite.setPosition(current_px.x, current_px.y);
    window.draw(sprite);
}
