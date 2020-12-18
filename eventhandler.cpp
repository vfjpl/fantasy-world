#include "eventhandler.hpp"

#define PERIOD_IN_FRAMES(frames, fps) ((1000000 * (frames)) / (fps))
#define PERIOD_IN_MILISECONDS(miliseconds) (1000 * (miliseconds))
namespace
{
unsigned long pathToDir(Position start, Position stop)
{
    if(stop.first < start.first)
        return 1;
    if(stop.first > start.first)
        return 2;
    if(stop.second < start.second)
        return 3;
    if(stop.second > start.second)
        return 4;
    return 0;
}
}

Event EventHandler::pollEvent()
{
start:
    for(auto __begin = events.begin(), __end = events.end(); __begin != __end; ++__begin)
    {
        auto& event = *__begin;
        sf::Time currentTime = clock.getElapsedTime();
        if(currentTime >= event.time)
        {
            switch(event.code)
            {
            case Event::MOVE:
            {
                if(directions.empty() && path.empty())
                {
                    events.erase(__begin);
                    goto start;
                }
                event.time = currentTime + sf::microseconds(PERIOD_IN_FRAMES(16, 60));
                return event.code;
            }
            case Event::ATTACK:
            {
                if(attack_id)
                {
                    event.time = currentTime + sf::microseconds(PERIOD_IN_MILISECONDS(1000));
                    return event.code;
                }
                events.erase(__begin);
                goto start;
            }
            default:
            {
                break;
            }
            }// end switch
        }
    }
    return Event::NONE;
}

void EventHandler::goToPos(Map& map, Position start, Position goal)
{
    path.clear();
    if(start == goal)
        return;
    if(map.isObstacle(goal.first - 1, goal.second - 1))
        return;
    path.emplace(goal, 0ul);
    std::deque<Position> frontier;
    for(Position current = goal;;)
    {
        for(auto next: {Position(current.first - 1, current.second),
                        Position(current.first, current.second - 1),
                        Position(current.first, current.second + 1),
                        Position(current.first + 1, current.second)})
        {
            if(map.isObstacle(next.first - 1, next.second - 1))
                continue;
            if(path.count(next))
                continue;

            path.emplace(next, pathToDir(next, current));
            frontier.emplace_back(next);
        }

        if(frontier.empty())
        {
            path.clear();
            break;
        }

        current = frontier.front();
        frontier.pop_front();

        if(current == start)
        {
            startEvent(Event::MOVE);
            break;
        }
    }
}

void EventHandler::startMove(unsigned long dir)
{
    if(directions.empty())
    {
        path.clear();
        directions.emplace_back(dir);
        startEvent(Event::MOVE);
    }
    else
    {
        auto it_begin = directions.begin();
        auto it_end = directions.end();
        auto it_found = std::find(it_begin, it_end, dir);
        if(it_found == it_end)
            directions.emplace_back(dir);
    }
}

void EventHandler::stopMove(unsigned long dir)
{
    auto it_begin = directions.begin();
    auto it_end = directions.end();
    auto it_found = std::find(it_begin, it_end, dir);
    if(it_found != it_end)
        directions.erase(it_found);
}

unsigned long EventHandler::getDir(Position pos)
{
    if(directions.empty())
        return path[pos];
    return directions.back();
}

void EventHandler::startMonsterAttack(unsigned long id)
{
    if(attack_id == id)
    {
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
    attack_id = 0;
}

unsigned long EventHandler::getAttackId()
{
    return attack_id;
}

// private

void EventHandler::startEvent(Event code)
{
    auto it_begin = events.begin();
    auto it_end = events.end();
    auto it_found = std::find(it_begin, it_end, code);
    if(it_found == it_end)
        events.emplace_front(code);
}

void EventHandler::stopEvent(Event code)
{
    auto it_begin = events.begin();
    auto it_end = events.end();
    auto it_found = std::find(it_begin, it_end, code);
    if(it_found != it_end)
        events.erase(it_found);
}
