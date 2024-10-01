#ifndef LOCALPLAYER_HPP_INCLUDED
#define LOCALPLAYER_HPP_INCLUDED

#include <string>

struct LocalPlayer_t
{
	//32
	std::string looktype;
	//8
	long id;
	long x;
	long y;
};

extern LocalPlayer_t LocalPlayer;

#endif // LOCALPLAYER_HPP_INCLUDED
