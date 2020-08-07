#include "interface.hpp"
#include <TGUI/SignalImpl.hpp>
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Button.hpp>

void Interface::setup(sf::RenderWindow& window)
{
    gui.setTarget(window);
    chatbox = tgui::ChatBox::create();
    chatbox->setSize("100%", "50%");
}

void Interface::loginScreen(Network* network, LocalPlayer* localPlayer, sf::Vector2u windowSize)
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
        if(network->credentials(editBoxUsername->getText(), editBoxPassword->getText()))
        {
            gui.removeAllWidgets();
            selectHeroScreen(network, localPlayer, windowSize);
        }
    });

    gui.add(editBoxUsername);
    gui.add(editBoxPassword);
    gui.add(button);
}

void Interface::chatMessage(const Poco::DynamicAny& data)
{
    addChatLine(data["player"] + data["message"]);
}

bool Interface::handleEvent(const sf::Event& event)
{
    return gui.handleEvent(event);
}

void Interface::draw()
{
    gui.draw();
}

// private

void Interface::selectHeroScreen(Network* network, LocalPlayer* localPlayer, sf::Vector2u windowSize)
{
    auto listBox = network->getHeroesListBox();
    listBox->setPosition("50% - width/2", "50% - height");

    auto button = tgui::Button::create("Select");
    button->setPosition("50% - width/2", "50%");
    button->connect(tgui::Signals::Button::Pressed, [=]
    {
        network->selectHero(listBox->getSelectedItem());
        gui.removeAllWidgets();
        gameScreen(network, localPlayer, windowSize);
    });

    gui.add(listBox);
    gui.add(button);
}

void Interface::gameScreen(Network* network, LocalPlayer* localPlayer, sf::Vector2u windowSize)
{
    localPlayer->looktype = network->startGame(windowSize);

    auto editbox = tgui::EditBox::create();
    editbox->setSize("100%", editbox->getSize().y);
    editbox->setPosition("0%", "100% - height");
    editbox->connect(tgui::Signals::EditBox::ReturnKeyPressed, [=](const std::string& text)
    {
        editbox->setText(sf::String());
        network->message(text);
    });

    auto chatwindow = tgui::ChildWindow::create();
    chatwindow->setResizable();
    chatwindow->add(chatbox);
    chatwindow->add(editbox);

    gui.add(chatwindow);
}

void Interface::addChatLine(const std::string& line)
{
    chatbox->addLine(line);
}
