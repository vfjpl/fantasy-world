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
	timedEvent(Event c):code(c) {}
	bool operator==(Event c) const
	{
		return code == c;
	}
};

typedef std::pair<unsigned long, unsigned long> Position;

class EventHandler_t
{
	//80
	std::deque<timedEvent> events;
	std::deque<unsigned long> directions;
	//48
	std::map<Position, unsigned long> path;
	//8
	sf::Clock sfClock;
	unsigned long attack_id;


	void startEvent(Event code);
	void stopEvent(Event code);

public:
	Event pollEvent();

	void startMove(unsigned long dir);
	void stopMove(unsigned long dir);
	void startMovePath(unsigned long x, unsigned long y);
	void stopMoveEvent();
	unsigned long getDir();

	void startMonsterAttack(unsigned long id);
	void stopMonsterAttack();
	unsigned long getAttackId();
};

extern EventHandler_t EventHandler;

#endif // EVENTHANDLER_HPP_INCLUDED
