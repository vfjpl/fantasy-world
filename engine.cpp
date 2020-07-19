#include "engine.hpp"
#include "helperfunctions.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>

// view-source:http://alkatria.pl/templates/client/default/js/game.js

namespace
{
unsigned long var2int(const Poco::DynamicAny& var)
{
    if(var.isNumeric())
        return var;
    return str2int(var);
}
}

void Engine::setup()
{
    setup_window(false);
    inteface.loginScreen(&network, &map, window.getSize());
}

bool Engine::run_game()
{
    process_input();
    game_logic();
    draw_frame();

    return window.isOpen();
}

bool Engine::run_network()
{
    process_network(network.receive());

    return window.isOpen();
}

// private

void Engine::setup_window(bool fullscreen)
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    if(fullscreen)
    {
        window.create(mode, "Fantasy World", sf::Style::Fullscreen);
    }
    else
    {
        mode.width = (mode.width*3)/4;
        mode.height = (mode.height*3)/4;
        window.create(mode, "Fantasy World", sf::Style::Close);
    }
    window.setKeyRepeatEnabled(false);
    inteface.setRenderTarget(window);
    map.setDefaultCamera(window.getDefaultView());
}

void Engine::process_input()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        //process events only if gui didn't consume them
        if(inteface.handleEvent(event))
            continue;

        switch(event.type)
        {
        case sf::Event::Closed:
        {
            window.close();
            break;
        }
        case sf::Event::KeyPressed:
        {
            eventHandler.keyPress(event.key.code);
            break;
        }
        case sf::Event::KeyReleased:
        {
            eventHandler.keyRelease(event.key.code);
            break;
        }
        case sf::Event::MouseButtonPressed:
        {
            break;
        }
        default:
        {
            break;
        }
        }//end switch
    }//end while
}

void Engine::game_logic()
{
    for(;;)
    {
        switch(eventHandler.pollEvent())
        {
        case NONE:
        {
            return;
        }
        case MOVE_LEFT:
        {
            break;
        }
        case MOVE_RIGHT:
        {
            break;
        }
        case MOVE_UP:
        {
            break;
        }
        case MOVE_DOWN:
        {
            break;
        }
        default:
        {
            break;
        }
        }//end switch
    }//end for
}

void Engine::draw_frame()
{
    window.clear();
    map.draw(window);
    inteface.draw();
    window.display();
}

void Engine::process_network(const Poco::DynamicAny& networkData)
{
    switch(var2int(networkData["code"]))
    {
    case 1://global chat message
    {
        break;
    }
    case 5://show damage
    {
        break;
    }
    case 10://other player movement
    {
        map.movePlayer(networkData);
        break;
    }
    case 54://remove monster
    {
        map.deleteMonster(networkData);
        break;
    }
    case 55://update map data
    {
        map.updateMapData(networkData["data"]);
        break;
    }
    case 71://other player join
    {
        map.addPlayer(networkData);
        break;
    }
    case 99://message from server
    {
        std::cout << networkData["message"].toString() << '\n';
        break;
    }
    case 100://first load map data
    {
        map.firstLoadMapData(networkData["data"]);
        break;
    }
    case 101://my movement
    {
        break;
    }
    case 102://my back movement
    {
        break;
    }
    case 877://remove map item
    {
        map.deleteMapItem(networkData);
        break;
    }
    case 878://new map item
    {
        map.addMapItem(networkData["item"]);
        break;
    }
    case 1030://my health
    {
        break;
    }
    case 1051://other player left
    {
        map.deletePlayer(networkData["player"]);
        break;
    }
    case 1504://other player yell
    {
        break;
    }
    case char2int("loot"):
    {
        break;
    }
    case char2int("teleport"):
    {
        map.loadMapData(networkData["data"]);
        break;
    }
    case char2int("load_game"):
    {
        process_network(network.receiveInit());
        break;
    }
    default:
    {
        std::cout << networkData["code"].toString() << '\n';
        break;
    }
    }//end switch
}
