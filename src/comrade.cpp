#include "comrade.hpp"
#include "helperfunctions.hpp"

void Comrade::setTexture(const sf::Texture& texture, long width, long height)
{
	rect.width = width;
	rect.height = height;
	sprite.setTextureRect(rect);
	sprite.setTexture(texture, false);
}

void Comrade::setDir(long dir)
{
	rect.left = (++steps % 4l) * rect.width;
	rect.top = (dir % 4l) * rect.height;
	sprite.setTextureRect(rect);
}

void Comrade::setPosition(long x, long y)
{
	move(x, y);
	current_px = desired_px;
}

void Comrade::move(long x, long y)
{
	desired_px.x = (x * 32l) - ((rect.width - 32l)/2l);
	desired_px.y = (y * 32l) - (rect.height - 32l);
}

void Comrade::draw(sf::RenderWindow& window)
{
	current_px += clamp(desired_px - current_px);
	sprite.setPosition(current_px.x, current_px.y);
	window.draw(sprite);
}
