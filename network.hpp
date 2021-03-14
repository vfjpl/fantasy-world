#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/DynamicStruct.h>
#include <TGUI/Widgets/ListBox.hpp>

class Network
{
    //344
    static Poco::Net::HTTPSClientSession https;
    static Poco::Net::HTTPSClientSession wssHTTPS;
    //136
    static Poco::Net::HTTPRequest wssREQUEST;
    //112
    static Poco::Net::HTTPResponse wssRESPONSE;
    //32
    static Poco::Net::NameValueCollection cookies;
    static Poco::Buffer<char> buffer;
    //8
    static Poco::Net::WebSocket* socket;

public:
    static bool credentials(const std::string& login, const std::string& password);
    static tgui::ListBox::Ptr getHeroesList();
    static void selectHero(const std::string& hero);
    static void startWebSocket();
    static void stopWebSocket();

    static Poco::DynamicAny receive(const std::string& token);
    static Poco::DynamicAny receive();

    static void attackMonster(unsigned long target_id);
    static void message(const sf::String& message);
    static void move(unsigned long dir);
    static void use(unsigned long slot, const char* type);
    static void takeLoot(unsigned long index);
    static void sendReloadPlayer(unsigned long player_id);
    static void pickUpItem(unsigned long x, unsigned long y);
    static void useElement(unsigned long x, unsigned long y);
    static void sendReload();
    static void attackPlayer(unsigned long target_id);
    static void openChest(unsigned long id);
    static void spell(unsigned long spell_id);
    static void spellMonster(unsigned long spell_id, unsigned long target_id);

private:
    static std::string loadGameData();
    static void sendStart(const std::string& token);
    static void send(const Poco::DynamicStruct& data, unsigned long code);
    static void send(const Poco::DynamicStruct& data, const char* code);
    static void sendJson(const std::string& json);
};

#endif // NETWORK_HPP_INCLUDED
