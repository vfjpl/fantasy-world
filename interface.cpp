#include "interface.hpp"
#include <TGUI/SignalImpl.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Button.hpp>

void Interface::setRenderTarget(sf::RenderWindow& window)
{
    gui.setTarget(window);
}

void Interface::loginScreen(Network* network)
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
        if(network->login1_credentials(editBoxPassword->getText(), editBoxUsername->getText()))
            selectHeroScreen(network);
    });

    gui.add(editBoxUsername);
    gui.add(editBoxPassword);
    gui.add(button);
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

void Interface::selectHeroScreen(Network* network)
{
    gui.removeAllWidgets();
    network->login2_getListOfIDs();
}
