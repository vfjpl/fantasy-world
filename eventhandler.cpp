#include "eventhandler.hpp"
#include <algorithm>

Event EventHandler::getEvent()
{
    while(table.front() < events.size())
    {
        Event code = events[table.front()++];
        if(!table[code])
        {
            addDelay(code);
            return code;
        }
        --table[code];
    }

    table.front() = 0;
    return NONE;
}

int EventHandler::getTargetID()
{
    return target;
}

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

// private

void EventHandler::startEvent(Event code)
{
    table[code] = 0;
    events.emplace_front(code);
}

void EventHandler::stopEvent(Event code)
{
    events.erase(std::find(events.cbegin(), events.cend(), code));
}

void EventHandler::addDelay(Event code)
{
    switch(code)
    {
    case MOVE_LEFT:
    case MOVE_RIGHT:
    case MOVE_UP:
    case MOVE_DOWN:
        table[code] = 15;
        break;
    case ATTACK_MONSTER:
    case ATTACK_PLAYER:
        table[code] = 119;
        break;
    default:
        break;
    }//end switch
}
