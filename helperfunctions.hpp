#ifndef HELPERFUNCTIONS_HPP_INCLUDED
#define HELPERFUNCTIONS_HPP_INCLUDED

int clamp(int val);
sf::Vector2i clamp(sf::Vector2i val);

unsigned long str2int(const std::string& str);

constexpr unsigned long char2int(const char* str)
{
    unsigned long res = 0;
    for(; *str != '\0'; ++str)
        res += *str;
    return res;
}

#endif // HELPERFUNCTIONS_HPP_INCLUDED
