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
    //16
    Poco::Net::WebSocket socket;

public:
    Network();

    bool credentials(const std::string& login, const std::string& password);
    tgui::ListBox::Ptr getHeroesListBox();
    void selectHero(const std::string& hero);
    std::string getToken(LocalPlayer* localplayer);
    void sendInit(const std::string& token, sf::Vector2u windowSize);
    Poco::DynamicAny receiveInit(const std::string& token);
    Poco::DynamicAny receive();

    void attackMonster(unsigned long target_id);
    void message(const std::string& message);
    void move(unsigned long dir);
    void shortcut(unsigned long slot);
    void takeLoot();
    void attackPlayer(unsigned long target_id);
    void spell(unsigned long spell_id);
    void spellMonster(unsigned long spell_id, unsigned long target_id);

private:
    void send(const Poco::DynamicStruct& data, unsigned long code);
    void send(const Poco::DynamicStruct& data, const char* code);
    void send(const std::string& json);
};

#endif // NETWORK_HPP_INCLUDED
