#include "timer.hpp"
#include <algorithm>

Event Timer::getEvent()
{
    while(it != events.cend())
    {
        Event code = *it++;
        if(!delaytable[code])
        {
            addDelay(code);
            return code;
        }
        --delaytable[code];
    }
    it = events.cbegin();
    return NONE;
}

void Timer::startEvent(Event code)
{
    delaytable[code] = 0;
    events.emplace_front(code);
}

void Timer::stopEvent(Event code)
{
    events.erase(std::find(events.cbegin(), events.cend(), code));
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
        delaytable[code] = 15;
        break;
    case ATTACK:
        delaytable[code] = 119;
        break;
    default:
        break;
    }//end switch
}
