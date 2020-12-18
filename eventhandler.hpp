#ifndef EVENTHANDLER_HPP_INCLUDED
#define EVENTHANDLER_HPP_INCLUDED

#include "map.hpp"

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
    //48
    std::map<Position, unsigned long> path;
    //8
    sf::Clock clock;
    unsigned long attack_id;

public:
    Event pollEvent();

    void goToPos(Map& map, Position start, Position goal);
    void startMove(unsigned long dir);
    void stopMove(unsigned long dir);
    unsigned long getDir(Position pos);
    void startMonsterAttack(unsigned long id);
    void stopMonsterAttack();
    unsigned long getAttackId();

private:
    void startEvent(Event code);
    void stopEvent(Event code);
};

#endif // EVENTHANDLER_HPP_INCLUDED
