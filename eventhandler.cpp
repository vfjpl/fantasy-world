#include "eventhandler.hpp"
#include <algorithm>

Event EventHandler::pollEvent()
{
    while(table.front() < events.size())
    {
        Event code = events[table.front()];
        ++table.front();
        if(table[code])
        {
            --table[code];
        }
        else
        {
            resetTable(code);
            return code;
        }
    }

    resetTable(NONE);
    return NONE;
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

void EventHandler::mousePress()
{

}

// private

void EventHandler::startEvent(Event code)
{
    events.emplace_front(code);
}

void EventHandler::stopEvent(Event code)
{
    if(!events.empty())
        events.erase(std::find(events.cbegin(), events.cend(), code));
}

void EventHandler::resetTable(Event code)
{
    switch(code)
    {
    case NONE:
        table.front() = 0;
        break;
    case MOVE_LEFT:
    case MOVE_RIGHT:
    case MOVE_UP:
    case MOVE_DOWN:
        table[code] = 15;
        break;
    case ATTACK_MONSTER:
    case ATTACK_PLAYER:
        table[code] = 59;
        break;
    default:
        break;
    }//end switch
}
