#include <SFML/Config.hpp>
#include <string>

int clamp(int val)
{
    return std::max(-2, std::min(val, 2));
}

int str2int(const std::string& str)
{
    int res = 0;
    for(sf::Uint8 i = 0; i < str.size(); ++i)
        res -= str[i];
    return res;
}
