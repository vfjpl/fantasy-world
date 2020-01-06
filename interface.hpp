#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include "network.hpp"
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/ProgressBar.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFML/System/Clock.hpp>

class Engine;

class Interface
{
    //136
    sfg::Desktop desktop;
    //16
    sfg::Label::Ptr chatBoxMessages;
    sfg::Window::Ptr chatBoxWindow;
    sfg::ProgressBar::Ptr healthBar;
    sfg::ProgressBar::Ptr expBar;
    //8
    sf::Clock clock;
    //1
    sfg::SFGUI sfgui;
    bool captureEvents;

public:
    void setup(Network* network);
    void login_screen(Engine* engine, sf::Vector2u windowSize);
    void select_screen(Engine* engine, sf::Vector2u windowSize);
    void game_screen(Engine* engine, sf::Vector2u windowSize);

    void loadGameData(const Poco::DynamicStruct& data, Map& map);
    void healthChange(const Poco::DynamicStruct& data);
    void chatMessage(const Poco::DynamicStruct& data);
    bool handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);

private:
    void addChatLine(const std::string& line);
};

#endif // INTERFACE_HPP_INCLUDED
