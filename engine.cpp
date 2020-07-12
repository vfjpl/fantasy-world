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
    setup_window(false);
    network.login1_credentials("", "");
    network.login3_selectHero(network.login2_getListOfIDs()[0]);
    map.setPlayerLooktype(network.login4_getLookType());
    network.login5_sendInit(window.getSize());
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
    map.setDefaultCamera(window.getDefaultView());
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
            eventHandler.mousePress(map.getIDs(window, sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
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
    for(Event code = eventHandler.getEvent(); code != NONE; code = eventHandler.getEvent())
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
        case ATTACK_MONSTER:
        {
            network.attackMonster(eventHandler.getTargetID());
            break;
        }
        case ATTACK_PLAYER:
        {
            network.attackPlayer(eventHandler.getTargetID());
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
        map.movePlayer(networkData.extract<Poco::DynamicStruct>());
        break;
    }
    case 54://remove monster
    {
        map.deleteMonster(networkData.extract<Poco::DynamicStruct>());
        break;
    }
    case 55://update map data
    {
        map.updateMapData(networkData["data"].extract<Poco::DynamicStruct>());
        break;
    }
    case 71://other player join
    {
        map.addPlayer(networkData.extract<Poco::DynamicStruct>());
        break;
    }
    case 99://message from server
    {
        std::cout << networkData["message"].toString() << '\n';
        break;
    }
    case 100://init data
    {
        map.initData(networkData["data"].extract<Poco::DynamicStruct>());
        break;
    }
    case 101://my movement
    {
        map.moveMe(networkData.extract<Poco::DynamicStruct>());
        break;
    }
    case 102://my back movement
    {
        break;
    }
    case 877://remove map item
    {
        map.deleteMapItem(networkData.extract<Poco::DynamicStruct>());
        break;
    }
    case 878://new map item
    {
        map.addMapItem(networkData["item"].extract<Poco::DynamicStruct>());
        break;
    }
    case 1030://my health
    {
        break;
    }
    case 1051://other player left
    {
        map.deletePlayer(networkData["player"].extract<Poco::DynamicStruct>());
        break;
    }
    case 1504://other player yell
    {
        break;
    }
    case char2int("loot"):
    {
        eventHandler.stopMonsterAttackEvent();
        break;
    }
    case char2int("teleport"):
    {
        map.loadMapData(networkData["data"].extract<Poco::DynamicStruct>());
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
