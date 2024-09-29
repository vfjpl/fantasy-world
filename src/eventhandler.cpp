#include "eventhandler.hpp"
#include "map.hpp"
#include "localplayer.hpp"

#define PERIOD_IN_FRAMES(frames, fps) ((1000000 * (frames)) / (fps))
#define PERIOD_IN_MILISECONDS(miliseconds) (1000 * (miliseconds))


EventHandler_t EventHandler;


static sf::Time getPeriod(Event code)
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

static unsigned long positionsToDir(Position from, Position to)
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


timedEvent::timedEvent(Event c):
	code(c) {}


bool timedEvent::operator==(Event c) const
{
	return code == c;
}


void EventHandler_t::startEvent(Event code)
{
    auto it_begin = events.cbegin();
    auto it_end = events.cend();
    auto it_found = std::find(it_begin, it_end, code);
    if(it_found == it_end)
        events.emplace_front(code);
}

void EventHandler_t::stopEvent(Event code)
{
    auto it_begin = events.cbegin();
    auto it_end = events.cend();
    auto it_found = std::find(it_begin, it_end, code);
    if(it_found != it_end)
        events.erase(it_found);
}


Event EventHandler_t::pollEvent()
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


void EventHandler_t::startMove(unsigned long dir)
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

void EventHandler_t::stopMove(unsigned long dir)
{
    auto it_begin = directions.cbegin();
    auto it_end = directions.cend();
    auto it_found = std::find(it_begin, it_end, dir);
    if(it_found != it_end)
        directions.erase(it_found);
}

void EventHandler_t::startMovePath(unsigned long x, unsigned long y)
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

void EventHandler_t::stopMoveEvent()
{
    stopEvent(Event::MOVE);
    path.clear();
}

unsigned long EventHandler_t::getDir()
{
    if(!directions.empty())
        return directions.back();
    if(!path.empty())
        return path[Position(LocalPlayer.x, LocalPlayer.y)];
    return 0;
}

void EventHandler_t::startMonsterAttack(unsigned long id)
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

void EventHandler_t::stopMonsterAttack()
{
    if(attack_id != 0)
    {
        stopEvent(Event::ATTACK);
        attack_id = 0;
    }
}

unsigned long EventHandler_t::getAttackId()
{
    return attack_id;
}
