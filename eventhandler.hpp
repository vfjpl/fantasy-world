#ifndef EVENTHANDLER_HPP_INCLUDED
#define EVENTHANDLER_HPP_INCLUDED

#include <SFML/Window/Keyboard.hpp>
#include <deque>
#include <array>

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
    //8
    unsigned long target;
    //1
    std::array<sf::Uint8, EVENT_COUNT> table;

public:
    Event getEvent();
    unsigned long getTargetID();

    void keyPress(sf::Keyboard::Key code);
    void keyRelease(sf::Keyboard::Key code);
    void mousePress(unsigned long id);

    void stopMonsterAttackEvent();

private:
    void startEvent(Event code);
    void stopEvent(Event code);
    void addDelay(Event code);
};

#endif // EVENTHANDLER_HPP_INCLUDED
