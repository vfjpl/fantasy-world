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
    rect.top = (dir%4) * rect.height;
    rect.left = ((++steps)%4) * rect.width;
    sprite.setTextureRect(rect);
}

void Player::move(int x, int y)
{
    desired_px.x = (32 * x) - 32;
    desired_px.y = (32 * y) - 48;
}

void Player::set_position(int x, int y)
{
    move(x, y);
    current_px = desired_px;
}

void Player::set_position_server_bug(int x, int y)
{
    desired_px.x = (32 * x);
    desired_px.y = (32 * y) - 16;
    current_px = desired_px;
}

void Player::draw(sf::RenderWindow& window)
{
    current_px += clamp(desired_px - current_px);
    sprite.setPosition(current_px.x, current_px.y);
    window.draw(sprite);
}
