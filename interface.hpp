#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include <TGUI/Gui.hpp>
#include "network.hpp"
#include "map.hpp"

class Interface
{
    //248
    tgui::Gui gui;

public:
    void setRenderTarget(sf::RenderWindow& window);
    void loginScreen(Network* network, Map* map, sf::Vector2u windowSize);

    bool handleEvent(const sf::Event& event);
    void draw();

private:
    void selectHeroScreen(Network* network, Map* map, sf::Vector2u windowSize);
    void gameScreen(Network* network, Map* map, sf::Vector2u windowSize);
};

#endif // INTERFACE_HPP_INCLUDED
