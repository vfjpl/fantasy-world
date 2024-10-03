#ifndef COMRADE_HPP_INCLUDED
#define COMRADE_HPP_INCLUDED

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Comrade
{
	//288
	sf::Sprite sprite;
    //16
    sf::IntRect rect;
    //8
    sf::Vector2i current_px;
    sf::Vector2i desired_px;
    long steps;

public:
	void setTexture(const sf::Texture& texture, long width, long height);
	void setDir(long dir);
	void setPosition(long x, long y);
	void move(long x, long y);

	void draw(sf::RenderWindow& window);
};

#endif // COMRADE_HPP_INCLUDED
