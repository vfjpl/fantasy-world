#include "interface.hpp"
#include <TGUI/SignalImpl.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/ListBox.hpp>
#include <TGUI/Widgets/Button.hpp>

void Interface::setRenderTarget(sf::RenderWindow& window)
{
    gui.setTarget(window);
}

void Interface::loginScreen(Network* network, Map* map, sf::Vector2u windowSize)
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
        if(network->login1_credentials(editBoxUsername->getText(), editBoxPassword->getText()))
        {
            gui.removeAllWidgets();
            selectHeroScreen(network, map, windowSize);
        }
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

void Interface::selectHeroScreen(Network* network, Map* map, sf::Vector2u windowSize)
{
    auto listBox = tgui::ListBox::create();
    listBox->setPosition("50% - width/2", "50% - height");
    for(const auto& i: network->login2_getListOfIDs())
        listBox->addItem(i);

    auto button = tgui::Button::create("Select");
    button->setPosition("50% - width/2", "50%");
    button->connect(tgui::Signals::Button::Pressed, [=]
    {
        network->login3_selectHero(listBox->getSelectedItem());
        gui.removeAllWidgets();
        gameScreen(network, map, windowSize);
    });

    gui.add(listBox);
    gui.add(button);
}

void Interface::gameScreen(Network* network, Map* map, sf::Vector2u windowSize)
{
    map->setPlayerLooktype(network->login4_getLookType());
    network->login5_sendInit(windowSize);
}
