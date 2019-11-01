#include "monster.hpp"

Monster::Monster(const sf::Texture& texture):
    sprite(texture) {}

void Monster::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
