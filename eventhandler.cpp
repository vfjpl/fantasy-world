#include "eventhandler.hpp"
#include "map.hpp"
#include "localplayer.hpp"

struct timedEvent
{
    sf::Time time;
    Event code;
    timedEvent(Event c): code(c) {}
    bool operator==(Event c) const
    {
        return code == c;
    }
};
typedef std::pair<unsigned long, unsigned long> Position;

namespace
{
//80
std::deque<timedEvent> events;
std::deque<unsigned long> directions;
//48
std::map<Position, unsigned long> path;
//8
sf::Clock sfClock;
unsigned long attack_id;

void stopEvent(Event code)
{
    auto it_begin = events.cbegin();
    auto it_end = events.cend();
    auto it_found = std::find(it_begin, it_end, code);
    if(it_found != it_end)
        events.erase(it_found);
}

void startEvent(Event code)
{
    auto it_begin = events.cbegin();
    auto it_end = events.cend();
    auto it_found = std::find(it_begin, it_end, code);
    if(it_found == it_end)
        events.emplace_front(code);
}

unsigned long positionsToDir(Position from, Position to)
{
    if(from.second < to.second)
        return 4;
    if(from.second > to.second)
        return 3;
    if(from.first < to.first)
        return 2;
    if(from.first > to.first)
        return 1;
    return 0;
}

#define PERIOD_IN_FRAMES(frames, fps) ((1000000 * (frames)) / (fps))
#define PERIOD_IN_MILISECONDS(miliseconds) (1000 * (miliseconds))
sf::Time getPeriod(Event code)
{
    switch(code)
    {
    case Event::MOVE:
        return sf::microseconds(PERIOD_IN_FRAMES(16, 60));
    case Event::ATTACK:
        return sf::microseconds(PERIOD_IN_MILISECONDS(1000));
    default:
        return sf::Time::Zero;
    }//end switch
}
}//end namespace


Event EventHandler::pollEvent()
{
    sf::Time currentTime = sfClock.getElapsedTime();
    for(auto& event: events)
    {
        if(currentTime >= event.time)
        {
            event.time = currentTime + getPeriod(event.code);
            return event.code;
        }
    }
    return Event::NONE;
}


void EventHandler::startMove(unsigned long dir)
{
    if(directions.empty())
    {
        directions.emplace_back(dir);
        startEvent(Event::MOVE);
        path.clear();
    }
    else
    {
        auto it_begin = directions.cbegin();
        auto it_end = directions.cend();
        auto it_found = std::find(it_begin, it_end, dir);
        if(it_found == it_end)
            directions.emplace_back(dir);
    }
}

void EventHandler::stopMove(unsigned long dir)
{
    auto it_begin = directions.cbegin();
    auto it_end = directions.cend();
    auto it_found = std::find(it_begin, it_end, dir);
    if(it_found != it_end)
        directions.erase(it_found);
}

void EventHandler::startMovePath(unsigned long x, unsigned long y)
{
    if(Map::isObstacle(x - 1, y - 1))
        return;

    path.clear();
    std::deque<Position> frontier;
    frontier.emplace_front(Position(x, y));
    path.emplace(Position(x, y), 0ul);

    while(!frontier.empty())
    {
        Position current = frontier.front();
        frontier.pop_front();

        for(auto next:
                {
                    Position(current.first - 1, current.second),
                    Position(current.first, current.second - 1),
                    Position(current.first, current.second + 1),
                    Position(current.first + 1, current.second)
                })
        {
            if(Map::isObstacle(next.first - 1, next.second - 1))
                continue;
            if(path.count(next))
                continue;
            if(Map::isNpc(next.first, next.second))
                continue;

            path.emplace(next, positionsToDir(next, current));
            frontier.emplace_back(next);
        }
    }
    startEvent(Event::MOVE);
}

void EventHandler::stopMoveEvent()
{
    stopEvent(Event::MOVE);
    path.clear();
}

unsigned long EventHandler::getDir()
{
    if(!directions.empty())
        return directions.back();
    if(!path.empty())
        return path[Position(LocalPlayer::x, LocalPlayer::y)];
    return 0;
}

void EventHandler::startMonsterAttack(unsigned long id)
{
    if(attack_id == id)
    {
        stopEvent(Event::ATTACK);
        attack_id = 0;
    }
    else
    {
        attack_id = id;
        startEvent(Event::ATTACK);
    }
}

void EventHandler::stopMonsterAttack()
{
    if(attack_id != 0)
    {
        stopEvent(Event::ATTACK);
        attack_id = 0;
    }
}

unsigned long EventHandler::getAttackId()
{
    return attack_id;
}
