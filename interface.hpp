#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include "network.hpp"
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFML/System/Clock.hpp>

class Interface
{
    //136
    sfg::Desktop desktop;
    //16
    sfg::Label::Ptr chatBoxMessages;
    //8
    sf::Clock clock;
    //1
    sfg::SFGUI sfgui;
    bool captureEvents;

public:
    void setup();
    void login_screen(Network* network, sf::Vector2u windowSize);
    void select_screen(Network* network, sf::Vector2u windowSize);
    void game_screen(Network* network, sf::Vector2u windowSize);

    bool handleEvent(const sf::Event& event);
    void chatMessage(const Poco::DynamicStruct& data);
    void draw(sf::RenderWindow& window);

private:
    void addChatMessage(const std::string& message);
    void showChatBox(Network* network);
};

#endif // INTERFACE_HPP_INCLUDED
