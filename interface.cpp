#include "interface.hpp"

void Interface::setup(sf::Vector2u size)
{

}

void Interface::handleEvent(const sf::Event& event)
{
    desktop.HandleEvent(event);
}

void Interface::draw(sf::RenderWindow& window)
{
    desktop.Update(clock.restart().asSeconds());
    sfgui.Display(window);
}
