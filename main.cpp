#include "engine.hpp"
#include <SFML/System/Thread.hpp>

//view-source:http://alkatria.pl/game

int main()
{
    Engine engine;
    sf::Thread thread([&]
    {
        while(engine.run_network());
    });

    thread.launch();

    while(engine.run_game());

    return EXIT_SUCCESS;
}
