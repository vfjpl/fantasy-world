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
    rect.top = dir%4 * rect.height;
    sprite.setTextureRect(rect);
}

void Monster::set_position(int x, int y)
{
    move(x, y);
    sprite.setPosition(position.x, position.y);
}

void Monster::move(int x, int y)
{
    position.x = (32 * x) - ((rect.width - 32)/2);
    position.y = (32 * y) - (rect.height - 32);
}

void Monster::draw(sf::RenderWindow& window)
{
    sf::Vector2i diff = position - sf::Vector2i(sprite.getPosition());
    sprite.move(clamp(diff.x), clamp(diff.y));
    window.draw(sprite);
}
