#include "player.hpp"
#include "helperfunctions.hpp"
#include <SFML/Graphics/Texture.hpp>

void Player::setTexture(const sf::Texture& texture)
{
    frames = texture.getSize().x/32;
    rect.width = 32;
    rect.height = 48;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void Player::set_dir(unsigned long dir)
{
    ++steps;
    rect.left = (steps%frames) * rect.width;
    rect.top = (dir%4) * rect.height;
    sprite.setTextureRect(rect);
}

void Player::move(unsigned long x, unsigned long y)
{
    desired_px.x = (32 * x) - 32;
    desired_px.y = (32 * y) - 48;
}

void Player::set_position(unsigned long x, unsigned long y)
{
    move(x, y);
    current_px = desired_px;
}

bool Player::contains(sf::Vector2f coords)
{
    return sprite.getGlobalBounds().contains(coords);
}

void Player::draw(sf::RenderWindow& window)
{
    current_px += clamp(desired_px - current_px);
    sprite.setPosition(current_px.x, current_px.y);
    window.draw(sprite);
}
