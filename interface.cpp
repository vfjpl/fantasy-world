#include "interface.hpp"
#include "network.hpp"
#include "threads.hpp"
#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/ChatBox.hpp>
#include <TGUI/Widgets/ProgressBar.hpp>
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/SignalImpl.hpp>

//248
static tgui::Gui gui;
//16
static tgui::ChatBox::Ptr chatBox;
static tgui::ProgressBar::Ptr healthBar;


static void addChatLine(const std::string& line)
{
    chatBox->addLine(sf::String::fromUtf8(line.cbegin(), line.cend()));
}

static void gameScreen()
{
    Network::startWebSocket();
    networkThread.launch();

    //gui

    auto editbox = tgui::EditBox::create();
    editbox->setSize("100%", editbox->getFullSize().y);
    editbox->setPosition(0.f, "100% - height");
    editbox->connect(tgui::Signals::EditBox::ReturnKeyPressed, [=](const sf::String& text)
    {
        Network::message(text);
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

static void selectHeroScreen()
{
    auto listBox = Network::getHeroesList();
    listBox->setPosition("50% - width/2", "50% - height");

    auto button = tgui::Button::create("Select");
    button->setPosition("50% - width/2", "50%");
    button->connect(tgui::Signals::Button::Pressed, [=]
    {
        Network::selectHero(listBox->getSelectedItem());
        gui.removeAllWidgets();
        gameScreen();
    });

    gui.add(listBox);
    gui.add(button);
}


void Interface::setup(sf::RenderWindow& window)
{
    gui.setTarget(window);
    chatBox = tgui::ChatBox::create();
    healthBar = tgui::ProgressBar::create();
    healthBar->setPosition("50% - width/2", "100% - height");
}

void Interface::updateWindowSize(float width, float height)
{
    gui.setView(sf::View(sf::FloatRect(0, 0, width, height)));
}

void Interface::loginScreen()
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
        if(Network::credentials(editBoxUsername->getText(), editBoxPassword->getText()))
        {
            gui.removeAllWidgets();
            selectHeroScreen();
        }
    });

    gui.add(editBoxUsername);
    gui.add(editBoxPassword);
    gui.add(button);
}

void Interface::health(const Poco::DynamicAny& data)
{
    unsigned long health_max = data["health_max"];
    unsigned long health = data["health"];
    healthBar->setMaximum(health_max);
    healthBar->setValue(health);
}

void Interface::chatMessage(const Poco::DynamicAny& data)
{
    addChatLine(data["player"] + ": " + data["message"]);
}

bool Interface::handleEvent(const sf::Event& event)
{
    return gui.handleEvent(event);
}

void Interface::draw()
{
    gui.draw();
}
