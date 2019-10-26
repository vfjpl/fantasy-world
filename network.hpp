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

public:
    Network();
    std::string login(const std::string& login, const std::string& password);
    void send(const std::string& str);
    std::string receive();
};

#endif // NETWORK_HPP_INCLUDED
