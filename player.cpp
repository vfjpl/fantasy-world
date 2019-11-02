#include "player.hpp"

void Player::set_texture(const sf::Texture& texture)
{
    rect.width = 32;
    rect.height = 48;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void Player::set_position(int x, int y)
{
    sprite.setPosition((32 * x) - 32, (32 * y) - 48);
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
