#ifndef TIMER_HPP_INCLUDED
#define TIMER_HPP_INCLUDED

#include <SFML/Config.hpp>
#include <deque>
#include <array>

enum Event
{
    NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    ATTACK,
    EVENT_COUNT,
};

class Timer
{
    //80
    std::deque<Event> events;
    //EVENT_COUNT
    std::array<sf::Uint8, EVENT_COUNT> table;

public:
    Event getEvent();
    void startEvent(Event code);
    void stopEvent(Event code);

private:
    void addDelay(Event code);
};

#endif // TIMER_HPP_INCLUDED
