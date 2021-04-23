#ifndef EVENTHANDLER_HPP_INCLUDED
#define EVENTHANDLER_HPP_INCLUDED

enum class Event
{
    MOVE,
    ATTACK,
    NONE,
};

namespace EventHandler
{
Event pollEvent();

void startMove(unsigned long dir);
void stopMove(unsigned long dir);
void startMovePath(unsigned long x, unsigned long y);
void stopMoveEvent();
unsigned long getDir();

void startMonsterAttack(unsigned long id);
void stopMonsterAttack();
unsigned long getAttackId();
}

#endif // EVENTHANDLER_HPP_INCLUDED
