#include "interface.hpp"
#include "playerdata.hpp"
#include <SFGUI/Window.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Button.hpp>

namespace
{
sf::FloatRect getAllocation(sf::Vector2u windowSize)
{
    return sf::FloatRect((windowSize.x/2) - 63, (windowSize.y/2) - 63, 126, 0);
}
sf::Vector2f getPosition(sf::Vector2u windowSize)
{
    return sf::Vector2f((windowSize.x/2) - 63, (windowSize.y/2) - 63);
}
}

void Interface::setup()
{
    chatBoxMessages = sfg::Label::Create();
    chatBoxMessages->SetLineWrap(true);
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
        if(network->login(login_entry->GetText(), password_entry->GetText()))
        {
            desktop.RemoveAll();
            select_screen(network, windowSize);
        }
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
    window->SetAllocation(getAllocation(windowSize));

    desktop.Add(window);
}

void Interface::select_screen(Network* network, sf::Vector2u windowSize)
{
    auto label = sfg::Label::Create("Select Character:");
    auto combobox = sfg::ComboBox::Create();
    for(auto &i: network->getListOfIDs())
        combobox->AppendItem(i);
    auto select_button = sfg::Button::Create("select");
    select_button->GetSignal(sfg::Button::OnLeftClick).Connect([=]
    {
        network->selectHero(combobox->GetSelectedText());
        desktop.RemoveAll();
        game_screen(network, windowSize);
    });

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 1);
    box->Pack(label);
    box->Pack(combobox);
    box->Pack(select_button);

    auto window = sfg::Window::Create();
    window->Add(box);
    window->SetTitle("Fantasy World");
    window->SetPosition(getPosition(windowSize));

    desktop.Add(window);
}

void Interface::game_screen(Network* network, sf::Vector2u windowSize)
{
    PlayerData::looktype = network->getLookType();
    network->sendInit(windowSize);
    captureEvents = false;
    showChatBox(network);
}

bool Interface::handleEvent(const sf::Event& event)
{
    desktop.HandleEvent(event);
    return captureEvents;
}

void Interface::chatMessage(const Poco::DynamicStruct& data)
{
    addChatLine(data["player"] + ": " + data["message"] + "\n");
}

void Interface::draw(sf::RenderWindow& window)
{
    desktop.Update(clock.restart().asSeconds());
    sfgui.Display(window);
}

// private

void Interface::addChatLine(const std::string& line)
{
    chatBoxMessages->SetText(chatBoxMessages->GetText() + sf::String::fromUtf8(line.cbegin(), line.cend()));
}

void Interface::showChatBox(Network* network)
{
    auto scrolledWindow = sfg::ScrolledWindow::Create();
    scrolledWindow->AddWithViewport(chatBoxMessages);
    scrolledWindow->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_ALWAYS);

    auto entry = sfg::Entry::Create();
    auto button = sfg::Button::Create("send");
    button->GetSignal(sfg::Button::OnLeftClick).Connect([=]
    {
        network->message((const char*)entry->GetText().toUtf8().data());
        entry->SetText(sf::String());
    });

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 1);
    box->Pack(scrolledWindow);
    box->Pack(entry, false);
    box->Pack(button, false);

    auto window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
    window->Add(box);
    window->GetSignal(sfg::Window::OnCloseButton).Connect([=]
    {
        desktop.RemoveAll();
    });

    desktop.Add(window);
}
