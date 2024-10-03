#ifndef HELPERFUNCTIONS_HPP_INCLUDED
#define HELPERFUNCTIONS_HPP_INCLUDED

#include <Poco/DynamicAny.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>

sf::Vector2i clamp(sf::Vector2i val);

sf::String fromUtf8(std::string::const_iterator cbegin, std::string::const_iterator cend);

const char* asCharPtr(const Poco::DynamicAny& var);
std::string toString(const Poco::DynamicAny& var);
long toLong(const Poco::DynamicAny& var);

long hashVar(const Poco::DynamicAny& var);
long hashString(const std::string& str);
constexpr long hashCharPtr(const char* str)
{
	long res = 0;
	for(; *str != '\0'; ++str)
		res += *str;
	return res;
}

#endif // HELPERFUNCTIONS_HPP_INCLUDED
