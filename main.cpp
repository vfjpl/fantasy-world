#include "engine.hpp"

// view-source:http://alkatria.pl/game

int main()
{
    Engine engine;
    sf::Thread network_thread([&]
    {
        while(engine.run_network());
    });
    engine.setup(network_thread);

    while(engine.run_game());

    network_thread.terminate();
    return EXIT_SUCCESS;
}
