#ifndef LOCALPLAYER_HPP_INCLUDED
#define LOCALPLAYER_HPP_INCLUDED

#include <string>

struct LocalPlayer
{
    //32
    std::string looktype;
    //8
    unsigned long id;
    unsigned long x;
    unsigned long y;
};

#endif // LOCALPLAYER_HPP_INCLUDED
