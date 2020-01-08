#include "engine.hpp"
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Button.hpp>

namespace
{
sf::FloatRect LoginScreenAllocation(sf::Vector2u windowSize)
{
    return sf::FloatRect((windowSize.x/2) - 63, (windowSize.y/2) - 63, 126, 0);
}
sf::Vector2f SelectScreenPosition(sf::Vector2u windowSize)
{
    return sf::Vector2f((windowSize.x/2) - 63, (windowSize.y/2) - 63);
}
sf::FloatRect GameScreenAllocation(sf::Vector2u windowSize)
{
    return sf::FloatRect(0, windowSize.y - 90, windowSize.x, 90);
}
}

void Interface::setup(Network* network)
{
    chatBoxMessages = sfg::Label::Create();
    chatBoxMessages->SetLineWrap(true);

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

    chatBoxWindow = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
    chatBoxWindow->Add(box);
    chatBoxWindow->GetSignal(sfg::Window::OnCloseButton).Connect([=]
    {
        chatBoxWindow->Show(false);
    });

    healthBar = sfg::ProgressBar::Create();
    expBar = sfg::ProgressBar::Create();
    captureEvents = true;
}

void Interface::login_screen(Engine* engine, sf::Vector2u windowSize)
{
    auto login_label = sfg::Label::Create("Login:");
    auto login_entry = sfg::Entry::Create();
    auto password_label = sfg::Label::Create("Password:");
    auto password_entry = sfg::Entry::Create();
    password_entry->HideText('*');
    auto login_button = sfg::Button::Create("login");
    login_button->GetSignal(sfg::Button::OnLeftClick).Connect([=]
    {
        if(engine->network.login(login_entry->GetText(), password_entry->GetText()))
        {
            desktop.RemoveAll();
            select_screen(engine, windowSize);
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
    window->SetAllocation(LoginScreenAllocation(windowSize));

    desktop.Add(window);
}

void Interface::select_screen(Engine* engine, sf::Vector2u windowSize)
{
    auto label = sfg::Label::Create("Select Character:");
    auto combobox = sfg::ComboBox::Create();
    for(auto& i: engine->network.getListOfIDs())
        combobox->AppendItem(i);
    auto select_button = sfg::Button::Create("select");
    select_button->GetSignal(sfg::Button::OnLeftClick).Connect([=]
    {
        engine->network.selectHero(combobox->GetSelectedText());
        desktop.RemoveAll();
        game_screen(engine, windowSize);
    });

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 1);
    box->Pack(label);
    box->Pack(combobox);
    box->Pack(select_button);

    auto window = sfg::Window::Create();
    window->Add(box);
    window->SetTitle("Fantasy World");
    window->SetPosition(SelectScreenPosition(windowSize));

    desktop.Add(window);
}

void Interface::game_screen(Engine* engine, sf::Vector2u windowSize)
{
    engine->map.setPlayerLooktype(engine->network.getLookType());
    engine->network.sendInit(windowSize);
    captureEvents = false;

    auto chat_button = sfg::Button::Create("chat");
    chat_button->GetSignal(sfg::Button::OnLeftClick).Connect([=]
    {
        chatBoxWindow->Show(true);
    });
    auto logout_button = sfg::Button::Create("logout");

    auto v_box1 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 1);
    v_box1->Pack(healthBar);
    auto h_box2 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 1);
    h_box2->Pack(chat_button);
    h_box2->Pack(logout_button);
    auto v_box2 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 1);
    v_box2->Pack(h_box2);
    v_box2->Pack(expBar);
    auto v_box3 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 1);

    auto main_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 1);
    main_box->Pack(v_box1);
    main_box->Pack(v_box2);
    main_box->Pack(v_box3);

    auto window = sfg::Window::Create(sfg::Window::BACKGROUND);
    window->Add(main_box);
    window->SetAllocation(GameScreenAllocation(windowSize));

    desktop.Add(chatBoxWindow);
    desktop.Add(window);
}

void Interface::initPlayerData(const Poco::DynamicStruct& data)
{
    healthChange(data);
    float experience = data["experience"];
    float to_level = data["to_level"];
    expBar->SetFraction(experience/to_level);
}

void Interface::healthChange(const Poco::DynamicStruct& data)
{
    float health = data["health"];
    float health_max = data["health_max"];
    healthBar->SetFraction(health/health_max);
}

void Interface::chatMessage(const Poco::DynamicStruct& data)
{
    addChatLine(data["player"] + ": " + data["message"] + "\n");
}

bool Interface::handleEvent(const sf::Event& event)
{
    desktop.HandleEvent(event);
    return captureEvents;
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
