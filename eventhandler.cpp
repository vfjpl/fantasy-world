#include "eventhandler.hpp"
#include <algorithm>

void EventHandler::keyPress(sf::Keyboard::Key code)
{
    switch(code)
    {
    case sf::Keyboard::A:
        startEvent(MOVE_LEFT);
        break;
    case sf::Keyboard::D:
        startEvent(MOVE_RIGHT);
        break;
    case sf::Keyboard::S:
        startEvent(MOVE_DOWN);
        break;
    case sf::Keyboard::W:
        startEvent(MOVE_UP);
        break;
    default:
        break;
    }//end switch
}

void EventHandler::keyRelease(sf::Keyboard::Key code)
{
    switch(code)
    {
    case sf::Keyboard::A:
        stopEvent(MOVE_LEFT);
        break;
    case sf::Keyboard::D:
        stopEvent(MOVE_RIGHT);
        break;
    case sf::Keyboard::S:
        stopEvent(MOVE_DOWN);
        break;
    case sf::Keyboard::W:
        stopEvent(MOVE_UP);
        break;
    default:
        break;
    }//end switch
}

void EventHandler::mousePress()
{

}

// private

void EventHandler::startEvent(Event code)
{
    //events.emplace_front(code);
}

void EventHandler::stopEvent(Event code)
{
    //events.erase(std::find(events.cbegin(), events.cend(), code));
}
