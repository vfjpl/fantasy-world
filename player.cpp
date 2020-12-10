#include "player.hpp"
#include "helperfunctions.hpp"
#include <SFML/Graphics/Texture.hpp>

void Player::setTexture(const sf::Texture& texture)
{
    sf::Vector2u size = texture.getSize();
    frames_x = std::max(size.x/32, 1u);
    frames_y = std::max(size.y/48, 1u);
    rect.width = 32;
    rect.height = 48;
    sprite.setTextureRect(rect);
    sprite.setTexture(texture, false);
}

void Player::setDir(unsigned long dir)
{
    rect.left = (++steps % frames_x) * 32;
    rect.top = (dir % frames_y) * 48;
    sprite.setTextureRect(rect);
}

void Player::setPosition(unsigned long x, unsigned long y)
{
    move(x, y);
    current_px = desired_px;
}

void Player::move(unsigned long x, unsigned long y)
{
    desired_px.x = (32 * x) - 32;
    desired_px.y = (32 * y) - 48;
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
