#ifndef EVENTHANDLER_HPP_INCLUDED
#define EVENTHANDLER_HPP_INCLUDED

#include "map.hpp"
#include "localplayer.hpp"

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

    void startMove(unsigned long dir);
    void stopMove(unsigned long dir);
    void startMovePath(Map& map, LocalPlayer& localPlayer, unsigned long x, unsigned long y);
    void stopMoveEvent();
    unsigned long getDir(LocalPlayer& localPlayer);

    void startMonsterAttack(unsigned long id);
    void stopMonsterAttack();
    unsigned long getAttackId();

private:
    void startEvent(Event code);
    void stopEvent(Event code);
};

#endif // EVENTHANDLER_HPP_INCLUDED
