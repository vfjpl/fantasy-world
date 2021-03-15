#include "engine.hpp"
#include <SFML/System/Thread.hpp>

// view-source:http://alkatria.pl/game

sf::Thread networkThread([]
{
    while(Engine::run_network());
});

int main()
{
    Engine::setup();

    while(Engine::run_game());

    return EXIT_SUCCESS;
}
