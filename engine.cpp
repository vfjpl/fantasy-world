#include "engine.hpp"
#include "helperfunctions.hpp"
#include <iostream>

// view-source:http://alkatria.pl/templates/client/default/js/game.js

namespace
{
unsigned long var2int(const Poco::DynamicAny& var)
{
    if(var.isString())
        return str2int(var);
    return var;
}
}

void Engine::setup(sf::Thread& networkThread)
{
    setup_window();
    interface.setup(window);
    interface.loginScreen(&networkThread, &network, &localPlayer);
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
    mode.width = 1000;
    mode.height = 1000;
    window.create(mode, "Fantasy World");
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
        case sf::Event::Resized:
            map.updateWindowSize(event.size.width, event.size.height);
            interface.updateWindowSize(event.size.width, event.size.height);
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
    MapClickData data = map.mapMouseClick(window, point);
    if(data.chestID || data.monsterID || data.npcID || data.playerID || data.item || data.tile)
    {
        if(data.chestID)
            network.openChest(data.chestID);
        if(data.monsterID)
            eventHandler.startMonsterAttack(data.monsterID);
        if(data.item)
            network.pickUpItem(data.x, data.y);
        if(data.tile)
            network.useElement(data.x, data.y);
    }
    else
    {
        eventHandler.startMovePath(map, localPlayer, data.x, data.y);
    }
}

void Engine::mouseRelease(sf::Vector2i point)
{

}

void Engine::game_logic()
{
    switch(eventHandler.pollEvent())
    {
    case Event::MOVE:
        moveLocalPlayer(eventHandler.getDir(localPlayer));
        break;
    case Event::ATTACK:
        network.attackMonster(eventHandler.getAttackId());
        break;
    default:
        break;
    }//end switch
}

void Engine::moveLocalPlayer(unsigned long dir)
{
    if(dir)
        network.move(dir);
    else
        eventHandler.stopMoveEvent();
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
    case 5://show attack/damage
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
    case 100://first load map data
    {
        map.loadData_100(networkData["data"], localPlayer);
        break;
    }
    case 685://open chest (general update?)
    {
        map.openChest(networkData);
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
    case 964://show attack/damage
    {
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
    case char2int("move_me"):
    {
        map.moveLocalPlayer(networkData, localPlayer);
        break;
    }
    case char2int("reset_move"):
    {
        break;
    }
    case char2int("spell_effect"):
    {
        break;
    }
    case char2int("loot"):
    {
        eventHandler.stopMonsterAttack();
        break;
    }
    case char2int("move_outfit"):
    {
        map.moveOutfit(networkData);
        break;
    }
    case char2int("show_tile"):
    {
        map.updateTile(networkData);
        break;
    }
    case char2int("exhaust_tile"):
    {
        map.updateTile(networkData);
        break;
    }
    case char2int("multi_code"):
    {
        for(const auto& item: networkData["items"])
            process_network(item.isString() ? Poco::DynamicAny::parse(item) : item);
        break;
    }
    case char2int("json"):
    {
        process_network(network.receive(networkData["hash"]));
        break;
    }
    case char2int("load_game"):
    {
        process_network(network.receive(networkData["token"]));
        break;
    }
    case char2int("teleport"):
    {
        map.clear();
        map.loadData_teleport(networkData["data"], localPlayer);
        break;
    }
    case char2int("death"):
    {
        network.sendReload();
        break;
    }
    default:
    {
        std::cout << networkData.toString() << '\n';
        break;
    }
    }//end switch
}
