#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/WebSocket.h>

class Network
{
    Poco::Net::HTTPClientSession session;
    Poco::Net::HTTPRequest request;
    Poco::Net::HTTPResponse response;
    Poco::Net::WebSocket socket;
    Poco::Buffer<char> buffer;
    std::string cookies;
    std::string token;

public:
    Network();
    void login(const std::string& login, const std::string& password);
    std::string get_init();

    void send(const std::string& json);
    std::string receive();
};

#endif // NETWORK_HPP_INCLUDED
