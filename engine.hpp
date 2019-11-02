#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include "map.hpp"
#include "network.hpp"
#include "resource_manager.hpp"

class Engine
{
    sf::RenderWindow window;
    sf::View camera;
    Map map;
    Network network;
    Resource_Manager resourceManager;

public:
    Engine();
    bool run_game();
    bool run_network();

private:
    void setup_window(bool fullscreen);
    void cameraCenter(int x, int y);

    void process_input();
    void draw_frame();

    void process_network(const Poco::DynamicStruct& json);
};

#endif // ENGINE_HPP_INCLUDED
