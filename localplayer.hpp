#ifndef LOCALPLAYER_HPP_INCLUDED
#define LOCALPLAYER_HPP_INCLUDED

#include <deque>
#include <string>

struct LocalPlayer
{
    //80
    std::deque<unsigned long> directions;
    //32
    std::string looktype;
    //8
    unsigned long id;
    unsigned long target_id;

    void startMove(unsigned long dir);
    void stopMove(unsigned long dir);
};

#endif // LOCALPLAYER_HPP_INCLUDED
