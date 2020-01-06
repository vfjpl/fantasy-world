#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include "network.hpp"
#include "map.hpp"
#include "interface.hpp"
#include "timer.hpp"

class Engine
{
    //528
    sf::RenderWindow window;

public:
    Network network;
    Map map;
    Interface interface;
    Timer timer;

    void setup();
    bool run_game();
    bool run_network();

private:
    void setup_window(bool fullscreen);

    void process_input();
    void game_logic();
    void draw_frame();

    void process_network(const Poco::DynamicStruct& json);
};

#endif // ENGINE_HPP_INCLUDED
