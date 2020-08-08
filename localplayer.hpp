#ifndef LOCALPLAYER_HPP_INCLUDED
#define LOCALPLAYER_HPP_INCLUDED

#include <SFML/System/Vector2.hpp>
#include <string>

class LocalPlayer
{
public:
    //32
    std::string looktype;
    //8
    sf::Vector2i position;
    unsigned long target_id;
    unsigned long id;

public:
    sf::Vector2i nextPosition(unsigned long dir);
};

#endif // LOCALPLAYER_HPP_INCLUDED
