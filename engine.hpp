#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include "map.hpp"
#include "network.hpp"
#include "resource_manager.hpp"

class Engine
{
    sf::RenderWindow window;
    Network network;
    Map map;
    Resource_Manager resourceManager;
    sf::View camera;
    sf::Vector2i center;

public:
    void setup();
    bool run_game();
    bool run_network();

private:
    void setup_window(bool fullscreen);
    void cameraCenterInstant(int x, int y);
    void cameraCenterSmooth(int x, int y);

    void process_input();
    void scrool_camera();
    void draw_frame();

    void process_network(const Poco::DynamicStruct& json);

    void updateMap(const Poco::DynamicStruct& data);
    void loadData(const Poco::DynamicStruct& data);
};

#endif // ENGINE_HPP_INCLUDED
