#ifndef TIMER_HPP_INCLUDED
#define TIMER_HPP_INCLUDED

#include <SFML/Config.hpp>
#include <deque>

enum Event
{
    NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    ATTACK,
};

class Timer
{
    //80
    std::deque<Event> events;
    //32
    std::deque<Event>::iterator it;
    //5
    sf::Uint8 delaytable[5];

public:
    Event getEvent();
    void startEvent(Event code);
    void stopEvent(Event code);

private:
    void addDelay(Event code);
};

#endif // TIMER_HPP_INCLUDED
