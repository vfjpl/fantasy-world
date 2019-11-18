#include "player.hpp"
#include "helperfunctions.hpp"

void Player::setTexture(const sf::Texture& texture)
{
    rect.width = 32;
    rect.height = 48;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void Player::set_dir(int dir)
{
    rect.top = dir%4 * rect.height;
    sprite.setTextureRect(rect);
}

void Player::set_position(int x, int y)
{
    move(x, y);
    sprite.setPosition(position.x, position.y);
}

void Player::move(int x, int y)
{
    position.x = (32 * x) - 32;
    position.y = (32 * y) - 48;
}

void Player::draw(sf::RenderWindow& window)
{
    sf::Vector2i diff = position - sf::Vector2i(sprite.getPosition());
    sprite.move(clamp(diff.x), clamp(diff.y));
    window.draw(sprite);
}
