#include "interface.hpp"
#include "resourcemanager.hpp"

void Interface::setup(sf::Vector2u size)
{
    sf::IntRect rect;
    rect.width = size.x;
    rect.height = 90;
    elements.emplace_back(ResourceManager::getTexture("/templates/client/default/images/windows/box-bottom.png"));
    elements.back().setTextureRect(rect);
    elements.back().setPosition(0, size.y - 90);
}

void Interface::draw(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());

    for(auto &i: elements)
        window.draw(i);
}
