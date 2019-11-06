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
    Poco::Net::HTTPClientSession session;
    Poco::Net::HTTPRequest request;
    Poco::Net::HTTPResponse response;
    Poco::Net::WebSocket socket;
    Poco::Buffer<char> buffer;
    std::string looktype;
    std::string token;
    std::string cookies;

public:
    Network();
    void login(const std::string& login, const std::string& password);
    std::string getPlayerLooktype();

    void sendInit(sf::Vector2u windowSize);
    Poco::DynamicStruct receiveInit();

    void send(const std::string& json);
    Poco::DynamicStruct receive();

    void move(int dir);
};

#endif // NETWORK_HPP_INCLUDED
