#ifndef PLAYERDATA_HPP_INCLUDED
#define PLAYERDATA_HPP_INCLUDED

#include <SFML/System/Vector2.hpp>
#include <string>

class PlayerData
{
public:
    //32
    static std::string looktype;
    //8
    static sf::Vector2i position;
    //4
    static int id;
};

#endif // PLAYERDATA_HPP_INCLUDED
