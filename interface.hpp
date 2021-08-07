#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include <Poco/DynamicAny.h>
#include <SFML/Graphics/RenderWindow.hpp>

namespace Interface
{
void setup(sf::RenderWindow& window);
void loginScreen();

void health(const Poco::DynamicAny& data);
void chatMessage(const Poco::DynamicAny& data);

void updateWindowSize(float width, float height);
bool handleEvent(const sf::Event& event);
void draw();
}

#endif // INTERFACE_HPP_INCLUDED
