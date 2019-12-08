#include "interface.hpp"
#include <SFGUI/Widgets.hpp>

void Interface::login_screen(sf::Vector2u size)
{
    auto login_label = sfg::Label::Create(L"Login:");
    auto login_entry = sfg::Entry::Create();
    auto password_label = sfg::Label::Create(L"Password:");
    auto password_entry = sfg::Entry::Create();
    password_entry->HideText('*');
    auto login_button = sfg::Button::Create(L"login");
    login_button->GetSignal(sfg::Widget::OnLeftClick).Connect([&]
    {
        //network.login(login_entry->GetText(), password_entry->GetText());
    });

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    box->Pack(login_label);
    box->Pack(login_entry);
    box->Pack(password_label);
    box->Pack(password_entry);
    box->Pack(login_button);

    auto window = sfg::Window::Create();
    window->SetPosition(sf::Vector2f(size.x/2, size.y/2));
    window->Add(box);

    desktop.Add(window);
}

void Interface::handleEvent(const sf::Event& event)
{
    desktop.HandleEvent(event);
}

void Interface::draw(sf::RenderWindow& window)
{
    desktop.Update(clock.restart().asSeconds());
    sfgui.Display(window);
}
