#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/DynamicStruct.h>
#include <SFML/System/Vector2.hpp>

class Network
{
    //328
    Poco::Net::HTTPClientSession session;
    //136
    Poco::Net::HTTPRequest request;
    //112
    Poco::Net::HTTPResponse response;
    //32
    Poco::Buffer<char> buffer;
    std::string cookies;
    std::string token;
    //16
    Poco::Net::WebSocket socket;

public:
    Network();
    void login(const std::string& login, const std::string& password);
    void sendInit(sf::Vector2u windowSize);

    Poco::DynamicStruct receive();
    Poco::DynamicStruct receiveInit();

    void move(int dir);
    void attack(int id);

private:
    void send(const std::string& json);
};

#endif // NETWORK_HPP_INCLUDED
