#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include "network.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class Engine
{
    sf::RenderWindow window;
    Network network;

public:
    Engine();
    bool run_game();
    bool run_network();

private:
    void setup_window(bool fullscreen);

    void process_input();
    void draw_frame();
};

#endif // ENGINE_HPP_INCLUDED
