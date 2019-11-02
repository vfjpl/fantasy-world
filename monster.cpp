#include "monster.hpp"

void Monster::set_texture(const sf::Texture& texture, int width, int height)
{
    rect.width = width;
    rect.height = height;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void Monster::set_position(int x, int y)
{
    //view-source:http://fantasy-world.pl/templates/client/default/js/map.js
    sprite.setPosition((32 * x) - ((rect.width - 32)/2), (32 * y) - (rect.height - 32));
}

void Monster::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
