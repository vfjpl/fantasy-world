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

class timedEvent
{
public:
	sf::Time time;
	Event code;

	timedEvent(Event c);
	bool operator == (Event c) const;
};

typedef std::pair<long, long> Position;

class EventHandler_t
{
	//80
	std::deque<timedEvent> events;
	std::deque<long> directions;
	//48
	std::map<Position, long> path;
	//8
	sf::Clock sfClock;
	long attack_id;


	void startEvent(Event code);
	void stopEvent(Event code);

public:
	Event pollEvent();

	void startMove(long dir);
	void stopMove(long dir);
	void startMovePath(long x, long y);
	void stopMoveEvent();
	long getDir();

	void startMonsterAttack(long id);
	void stopMonsterAttack();
	long getAttackId();
};

extern EventHandler_t EventHandler;

#endif // EVENTHANDLER_HPP_INCLUDED
