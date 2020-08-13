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
    interface.setup(window);
    map.setup(window.getDefaultView());

    interface.loginScreen(&network, &localPlayer, window.getSize());
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
    window.setFramerateLimit(60);
}

void Engine::process_input()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        //process event only if gui didn't consume it
        if(interface.handleEvent(event))
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
            mousePress(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
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
            moveLocalPlayer(1);
            break;
        }
        case MOVE_RIGHT:
        {
            moveLocalPlayer(2);
            break;
        }
        case MOVE_UP:
        {
            moveLocalPlayer(3);
            break;
        }
        case MOVE_DOWN:
        {
            moveLocalPlayer(4);
            break;
        }
        case ATTACK_MONSTER:
        {
            network.attackMonster(localPlayer.target_id);
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
    interface.draw();
    window.display();
}

void Engine::process_network(const Poco::DynamicAny& networkData)
{
    switch(var2int(networkData["code"]))
    {
    case 1://global chat message
    {
        interface.chatMessage(networkData);
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
        map.loadData_100(networkData["data"], localPlayer);
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
        eventHandler.stopEvent(ATTACK_MONSTER);
        network.takeLoot();
        break;
    }
    case char2int("teleport"):
    {
        map.clear();
        map.loadData_teleport(networkData["data"], localPlayer);
        break;
    }
    case char2int("load_game"):
    {
        process_network(network.receiveInit(networkData["token"]));
        break;
    }
    default:
    {
        std::cout << networkData["code"].toString() << '\n';
        break;
    }
    }//end switch
}

void Engine::moveLocalPlayer(unsigned long dir)
{
    sf::Vector2i pos = localPlayer.nextPosition(dir);
    if(map.isObstacle(pos.x - 1, pos.y - 1))
        return;
    map.moveLocalPlayer(localPlayer.id, pos.x, pos.y, dir);
    localPlayer.position = pos;
    network.move(dir);
}

void Engine::mousePress(sf::Vector2i point)
{
    struct objectsIDs IDs = map.getObjectsIDs(window, point);
    if(IDs.monsterID)
    {
        localPlayer.target_id = IDs.monsterID;
        eventHandler.startEvent(ATTACK_MONSTER);
    }
}
