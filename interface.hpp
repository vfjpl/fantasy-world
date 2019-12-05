#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include <SFGUI/Desktop.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFML/System/Clock.hpp>

class Interface
{
    //136
    sfg::Desktop desktop;
    //8
    sf::Clock clock;
    //1
    sfg::SFGUI sfgui;

public:
    void setup(sf::Vector2u size);
    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);
};

#endif // INTERFACE_HPP_INCLUDED
