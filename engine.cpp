#include "engine.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>

namespace
{
constexpr int char2int(const char* str)
{
    int res = 0;
    for(sf::Uint8 i = 0; i < strlen(str); ++i)
        res -= str[i];
    return res;
}
int str2int(const std::string& str)
{
    int res = 0;
    for(sf::Uint8 i = 0; i < str.size(); ++i)
        res -= str[i];
    return res;
}
int var2int(const Poco::DynamicAny& var)
{
    if(var.isInteger())
        return var;
    return str2int(var);
}
}

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
            }//end switch
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
        }//end switch
    }//end while
}

void Engine::draw_frame()
{
    window.display();
}

void Engine::process_network(const Poco::DynamicStruct& json)
{
    auto code = json["code"];
    switch(var2int(code))
    {
    case 100:
    {
        auto data = json["data"];
        auto map_data = data["map_data"];
        for(sf::Uint8 i = 0; i < map_data.size(); ++i)
        {
            auto map_tile = map_data[i];
            resourceManager.load_graphic(map_tile["source"], MAP_TILE);
        }
        break;
    }
    case char2int("load_game"):
    {
        process_network(network.receiveInit());
        break;
    }
    default:
    {
        std::cout << code.toString() << " NOT IMPLEMENTED\n";
        break;
    }
    }//end switch
}
