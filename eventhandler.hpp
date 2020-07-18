#ifndef EVENTHANDLER_HPP_INCLUDED
#define EVENTHANDLER_HPP_INCLUDED

#include <SFML/Window/Keyboard.hpp>
#include <deque>

enum Event
{
    NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    ATTACK_MONSTER,
    ATTACK_PLAYER,
    EVENT_COUNT,
};

class EventHandler
{
    //80
    std::deque<Event> events;

public:
    void keyPress(sf::Keyboard::Key code);
    void keyRelease(sf::Keyboard::Key code);
    void mousePress();

private:
    void startEvent(Event code);
    void stopEvent(Event code);
};

#endif // EVENTHANDLER_HPP_INCLUDED
