#ifndef EVENTHANDLER_HPP_INCLUDED
#define EVENTHANDLER_HPP_INCLUDED

#include <SFML/System/Clock.hpp>
#include <deque>
#include <map>

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
typedef std::pair<unsigned long, unsigned long> Position;

class EventHandler
{
    //80
    static std::deque<timedEvent> events;
    static std::deque<unsigned long> directions;
    //48
    static std::map<Position, unsigned long> path;
    //8
    static sf::Clock clock;
    static unsigned long attack_id;

public:
    static Event pollEvent();

    static void startMove(unsigned long dir);
    static void stopMove(unsigned long dir);
    static void startMovePath(unsigned long x, unsigned long y);
    static void stopMoveEvent();
    static unsigned long getDir();

    static void startMonsterAttack(unsigned long id);
    static void stopMonsterAttack();
    static unsigned long getAttackId();

private:
    static void startEvent(Event code);
    static void stopEvent(Event code);
};

#endif // EVENTHANDLER_HPP_INCLUDED
