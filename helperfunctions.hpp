#ifndef HELPERFUNCTIONS_HPP_INCLUDED
#define HELPERFUNCTIONS_HPP_INCLUDED

int clamp(int val);
sf::Vector2i clamp(sf::Vector2i val);
bool inRange(int val);
bool inRange(sf::Vector2i val);
int str2int(const std::string& str);

constexpr int char2int(const char* str)
{
    int res = 0;
    for(sf::Uint8 i = 0; str[i] != '\0'; ++i)
        res -= str[i];
    return res;
}

#endif // HELPERFUNCTIONS_HPP_INCLUDED
