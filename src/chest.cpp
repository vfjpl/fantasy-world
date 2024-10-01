#include "chest.hpp"

void Chest::setTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture, true);
}

void Chest::setPosition(long x, long y)
{
    sprite.setPosition((x * 32) - 32,
                       (y * 32) - 32);
}

bool Chest::contains(sf::Vector2f coords)
{
    return sprite.getGlobalBounds().contains(coords);
}

void Chest::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
