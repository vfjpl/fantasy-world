#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/ChatBox.hpp>
#include "network.hpp"
#include "localplayer.hpp"

class Interface
{
    //248
    tgui::Gui gui;
    //16
    tgui::ChatBox::Ptr chatbox;

public:
    void setup(sf::RenderWindow& window);

    void loginScreen(Network* network, LocalPlayer* localPlayer, sf::Vector2u windowSize);

    void chatMessage(const Poco::DynamicAny& data);
    bool handleEvent(const sf::Event& event);
    void draw();

private:
    void selectHeroScreen(Network* network, LocalPlayer* localPlayer, sf::Vector2u windowSize);
    void gameScreen(Network* network, LocalPlayer* localPlayer, sf::Vector2u windowSize);

    void addChatLine(const std::string& line);
};

#endif // INTERFACE_HPP_INCLUDED
