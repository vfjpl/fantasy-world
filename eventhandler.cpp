#include "eventhandler.hpp"
#include <algorithm>

namespace
{
#define PERIOD_IN_FRAMES(frames, fps) ((1000000 * (frames)) / (fps))
#define PERIOD_IN_MILISECONDS(miliseconds) (1000 * (miliseconds))
sf::Time getPeriod(Event code)
{
    switch(code)
    {
    case Event::MOVE:
        return sf::microseconds(PERIOD_IN_FRAMES(16, 60));
    case Event::ATTACK:
        return sf::microseconds(PERIOD_IN_MILISECONDS(1000));
    default:
        return sf::Time::Zero;
    }//end switch
}
}

Event EventHandler::pollEvent()
{
    sf::Time currentTime = clock.getElapsedTime();
    for(auto& event: events)
    {
        if(currentTime >= event.time)
        {
            event.time = currentTime + getPeriod(event.code);
            return event.code;
        }
    }
    return Event::NONE;
}

void EventHandler::startEvent(Event code)
{
    auto it_begin = events.cbegin();
    auto it_end = events.cend();
    auto it_found = std::find(it_begin, it_end, code);
    if(it_found == it_end)
        events.emplace_front(code);
}

void EventHandler::stopEvent(Event code)
{
    auto it_begin = events.cbegin();
    auto it_end = events.cend();
    auto it_found = std::find(it_begin, it_end, code);
    if(it_found != it_end)
        events.erase(it_found);
}

void EventHandler::startMove(unsigned long dir)
{
    if(directions.empty())
    {
        directions.emplace_back(dir);
        startEvent(Event::MOVE);
    }
    else
    {
        auto it_begin = directions.cbegin();
        auto it_end = directions.cend();
        auto it_found = std::find(it_begin, it_end, dir);
        if(it_found == it_end)
            directions.emplace_back(dir);
    }
}

void EventHandler::stopMove(unsigned long dir)
{
    auto it_begin = directions.cbegin();
    auto it_end = directions.cend();
    auto it_found = std::find(it_begin, it_end, dir);
    if(it_found != it_end)
        directions.erase(it_found);
    if(directions.empty())
        stopEvent(Event::MOVE);
}

unsigned long EventHandler::getDir()
{
    return directions.back();
}

void EventHandler::startMonsterAttack(unsigned long id)
{
    if(attack_id == id)
    {
        stopEvent(Event::ATTACK);
        attack_id = 0;
    }
    else
    {
        attack_id = id;
        startEvent(Event::ATTACK);
    }
}

void EventHandler::stopMonsterAttack()
{
    if(attack_id != 0)
    {
        stopEvent(Event::ATTACK);
        attack_id = 0;
    }
}

unsigned long EventHandler::getAttackId()
{
    return attack_id;
}
