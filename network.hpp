#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/DynamicStruct.h>
#include <TGUI/Widgets/ListBox.hpp>
#include "localplayer.hpp"

class Network
{
    //344
    Poco::Net::HTTPSClientSession https;
    Poco::Net::HTTPSClientSession wssHTTPS;
    //136
    Poco::Net::HTTPRequest wssREQUEST;
    //112
    Poco::Net::HTTPResponse wssRESPONSE;
    //32
    Poco::Net::NameValueCollection cookies;
    Poco::Buffer<char> buffer;
    //8
    Poco::Net::WebSocket* socket;

public:
    Network();

    bool credentials(const std::string& login, const std::string& password);
    tgui::ListBox::Ptr getHeroesList();
    void selectHero(const std::string& hero);
    void startWebSocket(LocalPlayer* localplayer, sf::Vector2u windowSize);

    Poco::DynamicAny receive(const std::string& token);
    Poco::DynamicAny receive();

    void attackMonster(unsigned long target_id);
    void message(const sf::String& message);
    void move(unsigned long dir);
    void use(unsigned long slot, const char* type);
    void takeLoot();
    void sendReloadPlayer(unsigned long player_id);
    void pickUpItem(unsigned long x, unsigned long y);
    void useElement(unsigned long x, unsigned long y);
    void sendReload();
    void attackPlayer(unsigned long target_id);
    void openChest(unsigned long id);
    void spell(unsigned long spell_id);
    void spellMonster(unsigned long spell_id, unsigned long target_id);

private:
    std::string loadGameData();
    void sendStart(const std::string& token, sf::Vector2u windowSize);
    void send(const Poco::DynamicStruct& data, unsigned long code);
    void send(const Poco::DynamicStruct& data, const char* code);
    void sendJson(const std::string& json);
};

#endif // NETWORK_HPP_INCLUDED
