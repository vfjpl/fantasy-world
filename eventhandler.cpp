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

void EventHandler::mousePress(int id)
{
    if(id)
        attackMonster(id);
}

void EventHandler::stopMonsterAttackEvent()
{
    stopEvent(ATTACK_MONSTER);
    target = 0;
}

// private

void EventHandler::startEvent(Event code)
{
    events.emplace_front(code);
}

void EventHandler::stopEvent(Event code)
{
    events.erase(std::find(events.cbegin(), events.cend(), code));
    table[code] = 0;
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
        table[code] = 59;
        break;
    default:
        break;
    }//end switch
}

void EventHandler::attackMonster(int id)
{
    if(target == id)
    {
        stopEvent(ATTACK_MONSTER);
        target = 0;
    }
    else if(target != 0)
    {
        target = id;
        table[ATTACK_MONSTER] = 0;
    }
    else
    {
        target = id;
        startEvent(ATTACK_MONSTER);
    }
}
