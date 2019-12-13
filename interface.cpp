#include "interface.hpp"
#include "playerdata.hpp"
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Box.hpp>

void Interface::setup(Network* network)
{
    chatBoxMessages = sfg::Label::Create();
    chatBoxMessages->SetLineWrap(true);

    auto scrolledWindow = sfg::ScrolledWindow::Create();
    scrolledWindow->AddWithViewport(chatBoxMessages);
    scrolledWindow->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_ALWAYS);

    auto chatBoxEntry = sfg::Entry::Create();
    auto chatBoxButton = sfg::Button::Create("send");
    chatBoxButton->GetSignal(sfg::Button::OnLeftClick).Connect([=]
    {
        network->message(chatBoxEntry->GetText());
        chatBoxEntry->SetText(sf::String());
    });

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    box->Pack(scrolledWindow);
    box->Pack(chatBoxEntry);
    box->Pack(chatBoxButton);

    chatBoxWindow = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
    chatBoxWindow->Add(box);
    chatBoxWindow->GetSignal(sfg::Window::OnCloseButton).Connect([=]
    {
        desktop.Remove(chatBoxWindow);
    });

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
    login_button->GetSignal(sfg::Button::OnLeftClick).Connect([=]
    {
        network->login(login_entry->GetText(), password_entry->GetText());
        network->selectHero(network->getListOfIDs());
        PlayerData::looktype = network->getLookType();
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
    chatBoxMessages->SetText(chatBoxMessages->GetText() + message + '\n');
}
