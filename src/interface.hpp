#ifndef INTERFACE_HPP_INCLUDED
#define INTERFACE_HPP_INCLUDED

#include <Poco/DynamicAny.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/ChatBox.hpp>
#include <TGUI/Widgets/ProgressBar.hpp>

class Interface_t
{
	//248
	tgui::Gui gui;
	//168
	sf::View camera;
	//16
	tgui::ChatBox::Ptr chatBox;
	tgui::ProgressBar::Ptr healthBar;


	void addChatLine(Poco::DynamicAny line);
	void gameScreen();
	void selectHeroScreen();

public:
	void setup(sf::RenderWindow& window);
	void loginScreen();

	void health(const Poco::DynamicAny& data);
	void chatMessage(const Poco::DynamicAny& data);

	void updateWindowSize(float width, float height);
	bool handleEvent(const sf::Event& event);
	void draw();
};

extern Interface_t Interface;

#endif // INTERFACE_HPP_INCLUDED
