#include "door.hpp"

Door::Door(const sf::Texture& texture):
    sprite(texture) {}


void Door::set_position(unsigned long x, unsigned long y)
{
    sprite.setPosition((32 * x) - 32, (32 * y) - 32);
}

void Door::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
