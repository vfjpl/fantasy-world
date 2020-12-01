#include "eventhandler.hpp"
#include <algorithm>

namespace
{
#define PERIOD_IN_FRAMES(frames, fps) ((1000000 * frames) / fps)
sf::Time getPeriod(Event code)
{
    switch(code)
    {
    case Event::MOVE_LEFT:
    case Event::MOVE_RIGHT:
    case Event::MOVE_UP:
    case Event::MOVE_DOWN:
        return sf::microseconds(PERIOD_IN_FRAMES(16, 60));
    case Event::ATTACK_MONSTER:
    case Event::ATTACK_PLAYER:
        return sf::microseconds(PERIOD_IN_FRAMES(60, 60));
    default:
        return sf::Time::Zero;
    }//end switch
}
}

Event EventHandler::pollEvent()
{
    sf::Time currentTime = clock.getElapsedTime();
    for(auto& tE: events)
    {
        if(currentTime >= tE.time)
        {
            tE.time = currentTime + getPeriod(tE.code);
            return tE.code;
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
