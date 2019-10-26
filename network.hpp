#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/JSON/Parser.h>
#include <SFML/System/Vector2.hpp>

class Network
{
    Poco::Net::HTTPClientSession session;
    Poco::Net::HTTPRequest request;
    Poco::Net::HTTPResponse response;
    Poco::Net::WebSocket socket;
    Poco::JSON::Parser parser;
    Poco::Buffer<char> buffer;
    std::string cookies;
    std::string token;

public:
    Network();
    void login(const std::string& login, const std::string& password);

    void load_send(sf::Vector2u size);
    Poco::JSON::Object::Ptr load_receive();

    void send(const std::string& json);
    Poco::JSON::Object::Ptr receive();
};

#endif // NETWORK_HPP_INCLUDED
