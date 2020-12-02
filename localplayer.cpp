#include "localplayer.hpp"
#include <algorithm>

void LocalPlayer::startMove(unsigned long dir)
{
    auto it_begin = directions.cbegin();
    auto it_end = directions.cend();
    auto it_found = std::find(it_begin, it_end, dir);
    if(it_found == it_end)
        directions.emplace_back(dir);
}

void LocalPlayer::stopMove(unsigned long dir)
{
    auto it_begin = directions.cbegin();
    auto it_end = directions.cend();
    auto it_found = std::find(it_begin, it_end, dir);
    if(it_found != it_end)
        directions.erase(it_found);
}
