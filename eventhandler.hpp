#ifndef EVENTHANDLER_HPP_INCLUDED
#define EVENTHANDLER_HPP_INCLUDED

#include <SFML/System/Clock.hpp>
#include <deque>

enum class Event
{
    MOVE,
    ATTACK,
    NONE,
};

struct timedEvent
{
    sf::Time time;
    Event code;
    timedEvent(Event c): code(c) {}
    bool operator==(Event c) const { return code == c; }
};

class EventHandler
{
    //80
    std::deque<timedEvent> events;
    //8
    sf::Clock clock;

public:
    Event pollEvent();
    void startEvent(Event code);
    void stopEvent(Event code);
};

#endif // EVENTHANDLER_HPP_INCLUDED
