#include "engine.hpp"
#include "helperfunctions.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>

//view-source:http://fantasy-world.pl/templates/client/default/js/game.js

namespace
{
int var2int(const Poco::DynamicAny& var)
{
    if(var.isInteger())
        return var;
    return str2int(var);
}
}

void Engine::setup()
{
    setup_window(true);
    network.login();
    network.sendInit(window.getSize());
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
        window.create(mode, "fantasy-world", sf::Style::Fullscreen);
    }
    else
    {
        mode.width = (mode.width*2)/3;
        mode.height = (mode.height*2)/3;
        window.create(mode, "fantasy-world", sf::Style::Close);
    }
    window.setKeyRepeatEnabled(false);
    map.initDefaultCamera(window.getDefaultView());
}

void Engine::process_input()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
        {
            window.close();
            break;
        }
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
            case sf::Keyboard::A:
            {
                timer.startEvent(MOVE_LEFT);
                break;
            }
            case sf::Keyboard::D:
            {
                timer.startEvent(MOVE_RIGHT);
                break;
            }
            case sf::Keyboard::S:
            {
                timer.startEvent(MOVE_DOWN);
                break;
            }
            case sf::Keyboard::W:
            {
                timer.startEvent(MOVE_UP);
                break;
            }
            case sf::Keyboard::Escape:
            {
                window.close();
                break;
            }
            default:
            {
                break;
            }
            }//end switch
            break;
        }
        case sf::Event::KeyReleased:
        {
            switch(event.key.code)
            {
            case sf::Keyboard::A:
            {
                timer.stopEvent(MOVE_LEFT);
                break;
            }
            case sf::Keyboard::D:
            {
                timer.stopEvent(MOVE_RIGHT);
                break;
            }
            case sf::Keyboard::S:
            {
                timer.stopEvent(MOVE_DOWN);
                break;
            }
            case sf::Keyboard::W:
            {
                timer.stopEvent(MOVE_UP);
                break;
            }
            default:
            {
                break;
            }
            }//end switch
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
    for(Event code = timer.getEvent(); code != NONE; code = timer.getEvent())
    {
        switch(code)
        {
        case MOVE_LEFT:
        {
            network.move(1);
            break;
        }
        case MOVE_RIGHT:
        {
            network.move(2);
            break;
        }
        case MOVE_UP:
        {
            network.move(3);
            break;
        }
        case MOVE_DOWN:
        {
            network.move(4);
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
    map.draw(window);
    window.display();
}

void Engine::process_network(const Poco::DynamicStruct& json)
{
    switch(var2int(json["code"]))
    {
    case 5://show damage
    {
        break;
    }
    case 10://other player movement
    {
        map.movePlayer(json);
        break;
    }
    case 54://remove monster
    {
        map.deleteMonster(json);
        break;
    }
    case 55://update map data
    {
        map.updateMapData(json["data"].extract<Poco::DynamicStruct>());
        break;
    }
    case 71://other player join
    {
        map.addPlayer(json);
        break;
    }
    case 100:
    case char2int("teleport"):
    {
        map.loadMapData(json["data"].extract<Poco::DynamicStruct>());
        break;
    }
    case 101://my movement
    {
        map.moveMe(json);
        break;
    }
    case 102://my back movement
    {
        break;
    }
    case 877://remove map item
    {
        map.deleteMapItem(json);
        break;
    }
    case 878://new map item
    {
        map.addMapItem(json["item"].extract<Poco::DynamicStruct>());
        break;
    }
    case 1030://my health
    {
        break;
    }
    case 1051://other player left
    {
        map.deletePlayer(json["player"].extract<Poco::DynamicStruct>());
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
    case char2int("load_game"):
    {
        process_network(network.receiveInit());
        break;
    }
    default:
    {
        std::cout << json["code"].toString() << '\n';
        break;
    }
    }//end switch
}
