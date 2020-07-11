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
    std::vector<std::string> login2_getListOfIDs();
    void login3_selectHero(const std::string& hero_id);
    std::string login4_getLookType();
    void login5_sendInit(sf::Vector2u windowSize);

    Poco::DynamicStruct receiveInit();
    Poco::DynamicStruct receive();

    void attackMonster(int id);
    void message(const char* message);
    void move(int dir);
    void shortcut(int nr);
    void takeLoot();
    void attackPlayer(int id);

    void spell(int nr);
    void spell(int nr, int id);

private:
    void send(const Poco::DynamicStruct& data, int nr);
    void send(const std::string& json);
};

#endif // NETWORK_HPP_INCLUDED
