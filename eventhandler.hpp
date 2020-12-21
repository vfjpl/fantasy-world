#ifndef EVENTHANDLER_HPP_INCLUDED
#define EVENTHANDLER_HPP_INCLUDED

#include <SFML/System/Clock.hpp>
#include <deque>

typedef std::pair<unsigned long, unsigned long> Position;

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
    std::deque<unsigned long> directions;
    //8
    sf::Clock clock;
    unsigned long attack_id;

public:
    Event pollEvent();

    void startMove(unsigned long dir);
    void stopMove(unsigned long dir);
    void stopAllMove();
    unsigned long getDir();

    void startMonsterAttack(unsigned long id);
    void stopMonsterAttack();
    unsigned long getAttackId();

private:
    void startEvent(Event code);
    void stopEvent(Event code);
};

#endif // EVENTHANDLER_HPP_INCLUDED
