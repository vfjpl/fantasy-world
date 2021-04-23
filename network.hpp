#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <Poco/DynamicAny.h>
#include <TGUI/Widgets/ListBox.hpp>

namespace Network
{
bool credentials(const std::string& login, const std::string& password);
tgui::ListBox::Ptr getHeroesList();
void selectHero(const std::string& hero);
void startWebSocket();
void stopWebSocket();

Poco::DynamicAny receive(const std::string& token);
Poco::DynamicAny receive();

void attackMonster(unsigned long target_id);
void message(const sf::String& message);
void move(unsigned long dir);
void use(unsigned long slot, const char* type);
void takeLoot(unsigned long index);
void sendReloadPlayer(unsigned long player_id);
void pickUpItem(unsigned long x, unsigned long y);
void useElement(unsigned long x, unsigned long y);
void sendReload();
void attackPlayer(unsigned long target_id);
void openChest(unsigned long id);
void spell(unsigned long spell_id);
void spellMonster(unsigned long spell_id, unsigned long target_id);
}

#endif // NETWORK_HPP_INCLUDED
