#include "engine.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>

Engine::Engine()
{
    setup_window(false);
    network.login();
    network.sendInit(window.getSize());
}

bool Engine::run_game()
{
    process_input();
    draw_frame();

    return window.isOpen();
}

bool Engine::run_network()
{
    process_network(network.receive());

    return window.isOpen();
}

void Engine::setup_window(bool fullscreen)
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    if(fullscreen)
    {
        window.create(mode, "fantasy-world", sf::Style::Fullscreen);
    }
    else
    {
        mode.width = (mode.width*2)/3;
        mode.height = (mode.height*2)/3;
        window.create(mode, "fantasy-world", sf::Style::Close);
    }
    window.setKeyRepeatEnabled(false);
    window.clear();
}

void Engine::process_input()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
            case sf::Keyboard::Escape:
                window.close();
                break;
            default:
                break;
            }// end switch
            break;
        }
        case sf::Event::Closed:
        {
            window.close();
            break;
        }
        default:
        {
            break;
        }
        }// end switch
    }// end while
}

void Engine::draw_frame()
{
    window.display();
}

void Engine::process_network(const Poco::DynamicStruct& json)
{
    std::cout << json["code"].toString() << '\n';
}
