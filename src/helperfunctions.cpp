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


std::string var2str(const Poco::DynamicAny& var)
{
	return var.toString();
}

unsigned long var2int(const Poco::DynamicAny& var)
{
	return var;
}


unsigned long var2hash(const Poco::DynamicAny& var)
{
	if(var.isString())
		return char2hash(&const_cast<Poco::DynamicAny&>(var).at(0));
	else
		return var;
}

unsigned long str2hash(const std::string& str)
{
	unsigned long res = 0;
	for(auto c: str)
		res += c;
	return res;
}
