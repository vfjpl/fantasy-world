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

    bool login(const std::string& login, const std::string& password);
    std::vector<std::string> getListOfIDs();
    void selectHero(const std::string& id);
    std::string getLookType();
    void sendInit(sf::Vector2u windowSize);

    Poco::DynamicStruct receiveInit();
    Poco::DynamicStruct receive();

    void attackMonster(int id);
    void message(const char* message);
    void move(int dir);
    void takeLoot();
    void attackPlayer(int id);

private:
    void send(const std::string& json);
};

#endif // NETWORK_HPP_INCLUDED
