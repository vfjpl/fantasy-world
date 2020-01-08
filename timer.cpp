#include "timer.hpp"
#include <algorithm>

Event Timer::getEvent()
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

void Timer::startEvent(Event code)
{
    table[code] = 0;
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
        table[code] = 15;
        break;
    case ATTACK:
        table[code] = 119;
        break;
    default:
        break;
    }//end switch
}
