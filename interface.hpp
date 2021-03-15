#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include <Poco/DynamicAny.h>
#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/ChatBox.hpp>
#include <TGUI/Widgets/ProgressBar.hpp>

class Interface
{
    //248
    static tgui::Gui gui;
    //16
    static tgui::ChatBox::Ptr chatBox;
    static tgui::ProgressBar::Ptr healthBar;

public:
    static void setup(sf::RenderWindow& window);
    static void updateWindowSize(float width, float height);

    static void loginScreen();

    static void health(const Poco::DynamicAny& data);
    static void chatMessage(const Poco::DynamicAny& data);

    static bool handleEvent(const sf::Event& event);
    static void draw();

private:
    static void selectHeroScreen();
    static void gameScreen();
    static void addChatLine(const std::string& line);
};

#endif // INTERFACE_HPP_INCLUDED
