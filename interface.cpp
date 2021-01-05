#include "interface.hpp"
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/SignalImpl.hpp>

void Interface::setup(sf::RenderWindow& window)
{
    gui.setTarget(window);
    chatBox = tgui::ChatBox::create();
    healthBar = tgui::ProgressBar::create();
}

void Interface::loginScreen(Network* network, LocalPlayer* localplayer, sf::Vector2u windowSize)
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
            selectHeroScreen(network, localplayer, windowSize);
        }
    });

    gui.add(editBoxUsername);
    gui.add(editBoxPassword);
    gui.add(button);
}

void Interface::health(const Poco::DynamicAny& data)
{

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

// private

void Interface::selectHeroScreen(Network* network, LocalPlayer* localplayer, sf::Vector2u windowSize)
{
    auto listBox = network->getHeroesList();
    listBox->setPosition("50% - width/2", "50% - height");

    auto button = tgui::Button::create("Select");
    button->setPosition("50% - width/2", "50%");
    button->connect(tgui::Signals::Button::Pressed, [=]
    {
        network->selectHero(listBox->getSelectedItem());
        gui.removeAllWidgets();
        gameScreen(network, localplayer, windowSize);
    });

    gui.add(listBox);
    gui.add(button);
}

void Interface::gameScreen(Network* network, LocalPlayer* localplayer, sf::Vector2u windowSize)
{
    network->sendInit(network->getToken(localplayer), windowSize);

    auto editbox = tgui::EditBox::create();
    editbox->setSize("100%", editbox->getFullSize().y);
    editbox->setPosition(0.f, "100% - height");
    editbox->connect(tgui::Signals::EditBox::ReturnKeyPressed, [=](const sf::String& text)
    {
        network->message(text);
        editbox->setText(sf::String());
    });

    chatBox->setSize("100%", bindTop(editbox));

    auto chatwindow = tgui::ChildWindow::create();
    chatwindow->setResizable();
    chatwindow->add(editbox);
    chatwindow->add(chatBox);

    gui.add(chatwindow);
}

void Interface::addChatLine(const std::string& line)
{
    chatBox->addLine(sf::String::fromUtf8(line.cbegin(), line.cend()));
}
