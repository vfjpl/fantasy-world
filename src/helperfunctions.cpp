#include "helperfunctions.hpp"


static int clamp(int val)
{
    return std::min(std::max(val, -2), 2);
}


sf::Vector2i clamp(sf::Vector2i val)
{
    return sf::Vector2i(clamp(val.x), clamp(val.y));
}


sf::String fromUtf8(std::string::const_iterator cbegin, std::string::const_iterator cend)
{
	return sf::String::fromUtf8(cbegin, cend);
}


const char* asCharPtr(const Poco::DynamicAny& var)
{
	return &const_cast<Poco::DynamicAny&>(var).at(0);
}

std::string toString(const Poco::DynamicAny& var)
{
	return var.toString();
}

long toLong(const Poco::DynamicAny& var)
{
	return var;
}


long hashVar(const Poco::DynamicAny& var)
{
	if(var.isString())
		return hashCharPtr(asCharPtr(var));
	else
		return toLong(var);
}

long hashString(const std::string& str)
{
	return hashCharPtr(str.c_str());
}
