#include "interface.hpp"
#include "network.hpp"
#include "threads.hpp"
#include "helperfunctions.hpp"
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/SignalImpl.hpp>
#include <iostream>

Interface_t Interface;


void Interface_t::addChatLine(Poco::DynamicAny line)
{
	const char* ptr_begin = asCharPtr(line);
	chatBox->addLine(fromUtf8(std::string::const_iterator(ptr_begin), std::string::const_iterator(ptr_begin + line.size())));
}

void Interface_t::gameScreen()
{
	gui.removeAllWidgets();

	auto editbox = tgui::EditBox::create();
	editbox->setSize("100%", editbox->getFullSize().y);
	editbox->setPosition(0.f, "100% - height");
	editbox->connect(tgui::Signals::EditBox::ReturnKeyPressed, [=](const sf::String& text)
	{
		Network.message(text);
		editbox->setText(sf::String());
	});

	chatBox->setSize("100%", bindTop(editbox));

	auto chatwindow = tgui::ChildWindow::create();
	chatwindow->setResizable();
	chatwindow->add(editbox);
	chatwindow->add(chatBox);

	gui.add(chatwindow);
	gui.add(healthBar);
}

void Interface_t::selectHeroScreen()
{
	gui.removeAllWidgets();

	auto listBox = Network.getHeroesList();
	listBox->setPosition("50% - width/2", "50% - height");

	auto button = tgui::Button::create("Select");
	button->setPosition("50% - width/2", "50%");
	button->connect(tgui::Signals::Button::Pressed, [=]
	{
		Network.selectHero(listBox->getSelectedItem());
		Network.startWebSocket();
		networkThread.launch();
		gameScreen();
	});

	gui.add(listBox);
	gui.add(button);
}


void Interface_t::setup(sf::RenderWindow& window)
{
	gui.setTarget(window);
	chatBox = tgui::ChatBox::create();
	healthBar = tgui::ProgressBar::create();
	healthBar->setPosition("50% - width/2", "100% - height");
}

void Interface_t::loginScreen()
{
	auto editBoxUsername = tgui::EditBox::create();
	editBoxUsername->setPosition("50% - width/2", "50% - height");
	editBoxUsername->setPasswordCharacter('*');

	auto editBoxPassword = tgui::EditBox::create();
	editBoxPassword->setPosition("50% - width/2", "50%");
	editBoxPassword->setPasswordCharacter('*');

	auto button = tgui::Button::create("Login");
	button->setPosition("50% - width/2", "50% + height");
	button->connect(tgui::Signals::Button::Pressed, [=]
	{
		if(Network.credentials(editBoxUsername->getText(), editBoxPassword->getText()))
		{
			selectHeroScreen();
		}
	});

	gui.add(editBoxUsername);
	gui.add(editBoxPassword);
	gui.add(button);
}

void Interface_t::health(const Poco::DynamicAny& data)
{
	long health_max = toLong(data["health_max"]);
	long health = toLong(data["health"]);
	healthBar->setMaximum(health_max);
	healthBar->setValue(health);
}

void Interface_t::chatMessage(const Poco::DynamicAny& data)
{
	std::cout << "chatMessage: " << toString(data) << '\n';
	// data["player"] vs data["name"] ??
	addChatLine(data["time"] + " [" + data["profession"] + "] " + data["player"] + " [" + data["level"] + "]: " + data["message"]);
}

void Interface_t::updateWindowSize(float width, float height)
{
	camera.reset(sf::FloatRect(0.f, 0.f, width, height));
	gui.setView(camera);
}

bool Interface_t::handleEvent(const sf::Event& event)
{
	return gui.handleEvent(event);
}

void Interface_t::draw()
{
	gui.draw();
}
