#include <SFML/System/Vector2.hpp>
#include <string>

int clamp(int val)
{
    return std::max(-2, std::min(val, 2));
}

sf::Vector2i clamp(sf::Vector2i val)
{
    return sf::Vector2i(clamp(val.x), clamp(val.y));
}

unsigned long str2int(const std::string& str)
{
    unsigned long res = 0;
    for(auto c: str)
        res += c;
    return res;
}
