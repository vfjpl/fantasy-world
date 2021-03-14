#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include <Poco/DynamicAny.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Thread.hpp>

class Engine
{
    //528
    static sf::RenderWindow window;

public:
    static void setup(sf::Thread& networkThread);

    static bool run_game();
    static bool run_network();

private:
    static void setup_window();


    static void process_input();
    static void keyPress(sf::Keyboard::Key code);
    static void keyRelease(sf::Keyboard::Key code);
    static void mousePress(sf::Vector2i point);
    static void mouseRelease(sf::Vector2i point);

    static void game_logic();
    static void moveLocalPlayer(unsigned long dir);

    static void draw_frame();

    static void process_network(const Poco::DynamicAny& networkData);
};

#endif // ENGINE_HPP_INCLUDED
