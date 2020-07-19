#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include <TGUI/Gui.hpp>
#include "network.hpp"

class Interface
{
    //248
    tgui::Gui gui;

public:
    void setRenderTarget(sf::RenderWindow& window);
    void loginScreen(Network* network, sf::Vector2u windowSize);

    bool handleEvent(const sf::Event& event);
    void draw();

private:
    void selectHeroScreen(Network* network, sf::Vector2u windowSize);
    void gameScreen(Network* network, sf::Vector2u windowSize);
};

#endif // INTERFACE_HPP_INCLUDED
