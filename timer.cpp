#include "timer.hpp"
#include <algorithm>

Event Timer::getEvent()
{
    while(it != events.end())
    {
        if(!delaytable[*it])
        {
            addDelay(*it);
            return *it++;
        }
        --delaytable[*it++];
    }
    it = events.begin();
    return NONE;
}

void Timer::startEvent(Event code)
{
    delaytable[code] = 0;
    events.emplace_front(code);
}

void Timer::stopEvent(Event code)
{
    events.erase(std::find(events.begin(), events.end(), code));
}

// private

void Timer::addDelay(Event code)
{
    switch(code)
    {
    case MOVE_LEFT:
    case MOVE_RIGHT:
    case MOVE_UP:
    case MOVE_DOWN:
        delaytable[code] = 16;
        break;
    case ATTACK:
        delaytable[code] = 120;
        break;
    default:
        break;
    }//end switch
}
