#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include <SFML/System/Thread.hpp>
#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/ChatBox.hpp>
#include <TGUI/Widgets/ProgressBar.hpp>
#include "network.hpp"

class Interface
{
    //248
    tgui::Gui gui;
    //16
    tgui::ChatBox::Ptr chatBox;
    tgui::ProgressBar::Ptr healthBar;

public:
    void setup(sf::RenderWindow& window);
    void updateWindowSize(float width, float height);

    void loginScreen(sf::Thread* networkThread, Network* network, LocalPlayer* localplayer);

    void health(const Poco::DynamicAny& data);
    void chatMessage(const Poco::DynamicAny& data);

    bool handleEvent(const sf::Event& event);
    void draw();

private:
    void selectHeroScreen(sf::Thread* networkThread, Network* network, LocalPlayer* localplayer);
    void gameScreen(sf::Thread* networkThread, Network* network, LocalPlayer* localplayer);
    void addChatLine(const std::string& line);
};

#endif // INTERFACE_HPP_INCLUDED
