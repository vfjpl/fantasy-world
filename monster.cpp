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
    position.x = (32 * x) - ((rect.width - 32)/2);
    position.y = (32 * y) - (rect.height - 32);
    sprite.setPosition(position.x, position.y);
}

void Monster::move(int x, int y)
{
    position.x = (32 * x) - ((rect.width - 32)/2);
    position.y = (32 * y) - (rect.height - 32);
}

void Monster::draw(sf::RenderWindow& window)
{
    sf::Vector2i diff = position - sf::Vector2i(sprite.getPosition());
    sprite.move(std::max(-1, std::min(diff.x, 1)), std::max(-1, std::min(diff.y, 1)));
    window.draw(sprite);
}
