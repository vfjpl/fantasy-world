#include "interface.hpp"
#include <SFGUI/Entry.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Box.hpp>

void Interface::setup()
{
    chatBoxMessages = sfg::Label::Create();
    chatBoxMessages->SetLineWrap(true);

    chatBoxWindow = sfg::Window::Create();
    chatBoxWindow->Add(chatBoxMessages);

    captureEvents = true;
}

void Interface::login_screen(Network* network, sf::Vector2u windowSize)
{
    auto login_label = sfg::Label::Create("Login:");
    auto login_entry = sfg::Entry::Create();
    auto password_label = sfg::Label::Create("Password:");
    auto password_entry = sfg::Entry::Create();
    password_entry->HideText('*');
    auto login_button = sfg::Button::Create("login");
    login_button->GetSignal(sfg::Widget::OnLeftClick).Connect([=]
    {
        network->login(login_entry->GetText(), password_entry->GetText());
        network->sendInit(windowSize);
        desktop.RemoveAll();
        captureEvents = false;
    });

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 1);
    box->Pack(login_label);
    box->Pack(login_entry);
    box->Pack(password_label);
    box->Pack(password_entry);
    box->Pack(login_button);

    auto window = sfg::Window::Create();
    window->Add(box);
    window->SetTitle("Fantasy World");
    window->SetAllocation(sf::FloatRect((windowSize.x/2) - 48, (windowSize.y/2) - 48, 96, 0));

    desktop.Add(window);
}

bool Interface::handleEvent(const sf::Event& event)
{
    desktop.HandleEvent(event);
    return captureEvents;
}

void Interface::chatMessage(const Poco::DynamicStruct& data)
{
    addChatMessage(data["message"]);
}

void Interface::draw(sf::RenderWindow& window)
{
    desktop.Update(clock.restart().asSeconds());
    sfgui.Display(window);
}

// private

void Interface::addChatMessage(const std::string& message)
{
    chatBoxMessages->SetText(chatBoxMessages->GetText() + '\n' + message);
}
