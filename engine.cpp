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
    setup_window();
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

void Engine::setup_window()
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    mode.width = (mode.width * 3) / 4;
    mode.height = (mode.height * 3) / 4;
    window.create(mode, "Fantasy World", sf::Style::Close);
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
            window.close();
            break;
        case sf::Event::KeyPressed:
            keyPress(event.key.code);
            break;
        case sf::Event::KeyReleased:
            keyRelease(event.key.code);
            break;
        case sf::Event::MouseButtonPressed:
            mousePress(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            break;
        case sf::Event::MouseButtonReleased:
            mouseRelease(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            break;
        default:
            break;
        }//end switch
    }//end while
}

void Engine::game_logic()
{
    switch(eventHandler.pollEvent())
    {
    case Event::MOVE:
        network.move(eventHandler.getDir());
        break;
    case Event::ATTACK:
        break;
    default:
        break;
    }//end switch
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
    //case 101://my movement
    //case 102://my back movement
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
    //case 1030://my health
    case 1051://other player left
    {
        map.deletePlayer(networkData["player"]);
        break;
    }
    case 1504://other player yell
    {
        break;
    }
    case char2int("move_me"):
    {
        map.moveLocalPlayer(networkData, localPlayer);
        break;
    }
    case char2int("reset_move"):
    {
        break;
    }
    //case char2int("loot"):
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

void Engine::keyPress(sf::Keyboard::Key code)
{
    switch(code)
    {
    case sf::Keyboard::A:
        eventHandler.startMove(1);
        break;
    case sf::Keyboard::D:
        eventHandler.startMove(2);
        break;
    case sf::Keyboard::S:
        eventHandler.startMove(4);
        break;
    case sf::Keyboard::W:
        eventHandler.startMove(3);
        break;
    case sf::Keyboard::Escape:
        break;
    default:
        break;
    }//end switch
}

void Engine::keyRelease(sf::Keyboard::Key code)
{
    switch(code)
    {
    case sf::Keyboard::A:
        eventHandler.stopMove(1);
        break;
    case sf::Keyboard::D:
        eventHandler.stopMove(2);
        break;
    case sf::Keyboard::S:
        eventHandler.stopMove(4);
        break;
    case sf::Keyboard::W:
        eventHandler.stopMove(3);
        break;
    case sf::Keyboard::Escape:
        window.close();
        break;
    default:
        break;
    }//end switch
}

void Engine::mousePress(sf::Vector2i point)
{

}

void Engine::mouseRelease(sf::Vector2i point)
{

}
