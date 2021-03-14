#include "engine.hpp"

// view-source:http://alkatria.pl/game

int main()
{
    sf::Thread network_thread([&]
    {
        while(Engine::run_network());
    });
    Engine::setup(network_thread);

    while(Engine::run_game());

    network_thread.terminate();
    return EXIT_SUCCESS;
}
