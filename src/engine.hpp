#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>

class Engine_t
{
	//528
	sf::RenderWindow window;


	void draw_frame();
	void mousePress(sf::Vector2i point);
	void process_input();
	void setup_window();

public:
	void setup();

	bool run_game();
	bool run_network();
};

extern Engine_t Engine;

#endif // ENGINE_HPP_INCLUDED
