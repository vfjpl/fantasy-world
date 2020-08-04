#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/DynamicStruct.h>
#include <SFML/System/Vector2.hpp>

class Network
{
    //344
    Poco::Net::HTTPSClientSession https;
    //328
    Poco::Net::HTTPClientSession http;
    //136
    Poco::Net::HTTPRequest request;
    //112
    Poco::Net::HTTPResponse response;
    //32
    Poco::Net::NameValueCollection cookies;
    Poco::Buffer<char> buffer;
    std::string token;
    //16
    Poco::Net::WebSocket socket;

public:
    Network();

    bool login1_credentials(const std::string& login, const std::string& password);
    std::vector<std::string> login2_getHeroesIDs();
    void login3_selectHero(const std::string& hero_id);
    std::string login4_getLooktype();
    void login5_sendInit(sf::Vector2u windowSize);

    Poco::DynamicAny receiveInit(const std::string& url_token);
    Poco::DynamicAny receive();

    void attackMonster(unsigned long target_id);
    void message(const char* message);
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
