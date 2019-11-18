#include "engine.hpp"
#include <SFML/System/Thread.hpp>

//view-source:http://fantasy-world.pl/game

int main()
{
    Engine engine;
    sf::Thread thread([&]
    {
        while(engine.run_network());
    });

    thread.launch();
    engine.setup();

    while(engine.run_game());

    return EXIT_SUCCESS;
}
