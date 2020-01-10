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
    //EVENT_COUNT
    std::array<sf::Uint8, EVENT_COUNT> table;
    //4
    int target;

public:
    Event getEvent();
    int getTargetID();

    void keyPress(sf::Keyboard::Key code);
    void keyRelease(sf::Keyboard::Key code);
    void mousePress(int id);

    void stopMonsterAttackEvent();

private:
    void startEvent(Event code);
    void stopEvent(Event code);
    void addDelay(Event code);
    void attackMonster(int id);
};

#endif // EVENTHANDLER_HPP_INCLUDED
