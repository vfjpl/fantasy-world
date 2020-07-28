#include "localplayer.hpp"

namespace
{
sf::Vector2i dirToNewPos(sf::Vector2i pos, unsigned long dir)
{
    switch(dir%4)
    {
    case 0:
        ++pos.y;
        break;
    case 1:
        --pos.x;
        break;
    case 2:
        ++pos.x;
        break;
    case 3:
        --pos.y;
        break;
    }
    return pos;
}
}

void LocalPlayer::set_position(unsigned long x, unsigned long y)
{
    position.x = x;
    position.y = y;
}

sf::Vector2i LocalPlayer::nextPosition(unsigned long dir)
{
    return dirToNewPos(position, dir);
}
