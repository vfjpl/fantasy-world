#ifndef HELPERFUNCTIONS_HPP_INCLUDED
#define HELPERFUNCTIONS_HPP_INCLUDED

#include <Poco/DynamicAny.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>

sf::Vector2i clamp(sf::Vector2i val);

sf::String fromUtf8(std::string::const_iterator cbegin, std::string::const_iterator cend);

std::string var2str(const Poco::DynamicAny& var);
const char* var2char(const Poco::DynamicAny& var);
unsigned long var2int(const Poco::DynamicAny& var);

unsigned long var2hash(const Poco::DynamicAny& var);
unsigned long str2hash(const std::string& str);
constexpr unsigned long char2hash(const char* str)
{
	unsigned long res = 0;
	for(; *str != '\0'; ++str)
		res += *str;
	return res;
}

#endif // HELPERFUNCTIONS_HPP_INCLUDED
