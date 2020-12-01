#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include "network.hpp"
#include "interface.hpp"
#include "map.hpp"
#include "localplayer.hpp"
#include "eventhandler.hpp"

class Engine
{
    //528
    sf::RenderWindow window;

    Network network;
    Interface interface;
    Map map;
    LocalPlayer localPlayer;
    EventHandler eventHandler;

public:
    void setup();

    bool run_game();
    bool run_network();

private:
    void setup_window(bool fullscreen);

    void process_input();
    void game_logic();
    void draw_frame();
    void process_network(const Poco::DynamicAny& networkData);

    void keyPress(sf::Keyboard::Key code);
    void keyRelease(sf::Keyboard::Key code);
    void mousePress(sf::Vector2i point);
    void mouseRelease(sf::Vector2i point);
};

#endif // ENGINE_HPP_INCLUDED
