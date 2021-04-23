#include "engine.hpp"
#include "network.hpp"
#include "interface.hpp"
#include "map.hpp"
#include "eventhandler.hpp"
#include "helperfunctions.hpp"
#include <iostream>

// view-source:http://alkatria.pl/templates/client/default/js/game.js

namespace
{
//528
sf::RenderWindow window;

unsigned long var2int(const Poco::DynamicAny& var)
{
    if(var.isString())
        return str2int(var);
    return var;
}

void process_network(const Poco::DynamicAny& networkData)
{
    switch(var2int(networkData["code"]))
    {
    case 1://global chat message
    {
        Interface::chatMessage(networkData);
        break;
    }
    case 5://show attack/damage
    {
        break;
    }
    case 10://other player movement
    {
        Map::movePlayer(networkData);
        break;
    }
    case 54://remove monster
    {
        Map::deleteMonster(networkData);
        break;
    }
    case 55://update map data
    {
        Map::updateMapData(networkData["data"]);
        break;
    }
    case 71://other player join
    {
        Map::addPlayer(networkData);
        break;
    }
    case 100://first load map data
    {
        Map::loadData_100(networkData["data"]);
        break;
    }
    case 654://gui update (general update?)
    {
        std::cout << networkData.toString() << '\n';
        Interface::health(networkData);
        break;
    }
    case 685://open chest (general update?)
    {
        std::cout << networkData.toString() << '\n';
        Map::openChest(networkData);
        break;
    }
    case 877://remove map item
    {
        Map::deleteMapItem(networkData);
        break;
    }
    case 878://new map item
    {
        Map::addMapItem(networkData["item"]);
        break;
    }
    case 964://monster health percentage
    {
        break;
    }
    case 1016:
    {
        std::cout << networkData.toString() << '\n';
        Interface::health(networkData);
        break;
    }
    case 1030://my health + message about dealt/received damage
    {
        Interface::health(networkData);
        break;
    }
    case 1051://other player left
    {
        Map::deletePlayer(networkData["player"]);
        break;
    }
    case 1504://other player yell
    {
        break;
    }
    case char2int("move_me"):
    {
        Map::moveLocalPlayer(networkData);
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
        EventHandler::stopMonsterAttack();
        break;
    }
    case char2int("move_outfit"):
    {
        Map::moveOutfit(networkData);
        break;
    }
    case char2int("show_tile"):
    {
        Map::updateTile(networkData);
        break;
    }
    case char2int("exhaust_tile"):
    {
        Map::updateTile(networkData);
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
        process_network(Network::receive(networkData["hash"]));
        break;
    }
    case char2int("load_game"):
    {
        process_network(Network::receive(networkData["token"]));
        break;
    }
    case char2int("teleport"):
    {
        Map::clear();
        Map::loadData_teleport(networkData["data"]);
        break;
    }
    case char2int("death"):
    {
        Network::sendReload();
        break;
    }
    default:
    {
        std::cout << networkData.toString() << '\n';
        break;
    }
    }//end switch
}

void draw_frame()
{
    window.clear();
    Map::draw(window);
    Interface::draw();
    window.display();
}

void moveLocalPlayer(unsigned long dir)
{
    if(dir)
        Network::move(dir);
    else
        EventHandler::stopMoveEvent();
}

void game_logic()
{
    switch(EventHandler::pollEvent())
    {
    case Event::MOVE:
        moveLocalPlayer(EventHandler::getDir());
        break;
    case Event::ATTACK:
        Network::attackMonster(EventHandler::getAttackId());
        break;
    default:
        break;
    }//end switch
}

void mouseRelease(sf::Vector2i point)
{

}

void mousePress(sf::Vector2i point)
{
    MapClickData data = Map::mapMouseClick(window, point);
    if(data.chestID || data.monsterID || data.npcID || data.playerID || data.tile || data.item)
    {
        if(data.chestID)
            Network::openChest(data.chestID);
        if(data.monsterID)
            EventHandler::startMonsterAttack(data.monsterID);
        if(data.tile)
            Network::useElement(data.x, data.y);
        if(data.item)
            Network::pickUpItem(data.x, data.y);
    }
    else
    {
        EventHandler::startMovePath(data.x, data.y);
    }
}

void keyRelease(sf::Keyboard::Key code)
{
    switch(code)
    {
    case sf::Keyboard::A:
        EventHandler::stopMove(1);
        break;
    case sf::Keyboard::D:
        EventHandler::stopMove(2);
        break;
    case sf::Keyboard::S:
        EventHandler::stopMove(4);
        break;
    case sf::Keyboard::W:
        EventHandler::stopMove(3);
        break;
    case sf::Keyboard::Escape:
        break;
    default:
        break;
    }//end switch
}

void keyPress(sf::Keyboard::Key code)
{
    switch(code)
    {
    case sf::Keyboard::A:
        EventHandler::startMove(1);
        break;
    case sf::Keyboard::D:
        EventHandler::startMove(2);
        break;
    case sf::Keyboard::S:
        EventHandler::startMove(4);
        break;
    case sf::Keyboard::W:
        EventHandler::startMove(3);
        break;
    case sf::Keyboard::Escape:
        break;
    default:
        break;
    }//end switch
}

void process_input()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        //process event only if gui didn't consume it
        if(Interface::handleEvent(event))
            continue;

        switch(event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::Resized:
            Map::updateWindowSize(event.size.width, event.size.height);
            Interface::updateWindowSize(event.size.width, event.size.height);
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

void setup_window()
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    mode.width = 1000;
    mode.height = 1000;
    window.create(mode, "Fantasy World");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);
}
}//end namespace


void Engine::setup()
{
    setup_window();
    Interface::setup(window);
    Interface::loginScreen();
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
    process_network(Network::receive());

    return window.isOpen();
}
