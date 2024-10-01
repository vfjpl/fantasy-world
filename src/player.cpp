#include "player.hpp"
#include "helperfunctions.hpp"
#include <SFML/Graphics/Texture.hpp>

void Player::setTexture(const sf::Texture& texture)
{
    rect.width = 32l;
    rect.height = 48l;
    sprite.setTextureRect(rect);
    sprite.setTexture(texture, false);
}

void Player::setDir(long dir)
{
    rect.left = (++steps % 4l) * 32l;
    rect.top = (dir % 4l) * 48l;
    sprite.setTextureRect(rect);
}

void Player::setPosition(long x, long y)
{
    move(x, y);
    current_px = desired_px;
}

void Player::move(long x, long y)
{
    desired_px.x = (x * 32l) - 32l;
    desired_px.y = (y * 32l) - 48l;
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
