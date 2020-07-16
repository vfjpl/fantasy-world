#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include "network.hpp"
#include "interface.hpp"
#include "map.hpp"
#include "eventhandler.hpp"

class Engine
{
    //528
    sf::RenderWindow window;

    Network network;
    Interface inteface;
    Map map;
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
};

#endif // ENGINE_HPP_INCLUDED
