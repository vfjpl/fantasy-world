#ifndef HELPERFUNCTIONS_HPP_INCLUDED
#define HELPERFUNCTIONS_HPP_INCLUDED

sf::Vector2i clamp(sf::Vector2i val);

unsigned long str2int(const std::string& str);

sf::String fromUtf8(std::string::const_iterator cbegin, std::string::const_iterator cend);

constexpr unsigned long char2int(const char* str)
{
    unsigned long res = 0;
    for(; *str != '\0'; ++str)
        res += *str;
    return res;
}

#endif // HELPERFUNCTIONS_HPP_INCLUDED
