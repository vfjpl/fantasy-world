#include "network.hpp"
#include <SFML/Network/Http.hpp>

namespace
{
std::string get_ID(const std::string& body)
{
    size_t pos = body.find("value") + 7;
    return body.substr(pos, body.find('\"', pos) - pos);
}
std::string get_COOKIE(const std::string& body)
{
    return body.substr(0, body.find(';'));
}
std::string get_PLAYER_TOKEN(const std::string& body)
{
    size_t pos = body.find("player_token") + 16;
    return body.substr(pos, body.find('\'', pos) - pos);
}
}

Network::Network():
    session("54.37.227.73", 9001),
    request(Poco::Net::HTTPRequest::HTTP_GET, "/echobot", Poco::Net::HTTPRequest::HTTP_1_1),
    socket(session, request, response),
    buffer(0) {}

void Network::login(const std::string& login, const std::string& password)
{
    sf::Http http("fantasy-world.pl");

    // 1 get first cookie
    sf::Http::Request request1("/modal/get/login");
    sf::Http::Response response1 = http.sendRequest(request1);
    cookies = get_COOKIE(response1.getField(Poco::Net::HTTPResponse::SET_COOKIE));

    // 2 log in and get second cookie
    sf::Http::Request request2("/ajax/login", sf::Http::Request::Post, "login=" + login + "&password=" + password);
    request2.setField(Poco::Net::HTTPRequest::COOKIE, cookies);
    sf::Http::Response response2 = http.sendRequest(request2);
    cookies += get_COOKIE(response2.getField(Poco::Net::HTTPResponse::SET_COOKIE));

    // 3 get hero id
    sf::Http::Request request3;
    request3.setField(Poco::Net::HTTPRequest::COOKIE, cookies);
    sf::Http::Response response3 = http.sendRequest(request3);

    // 4 set hero id
    sf::Http::Request request4("/game/login", sf::Http::Request::Post, "id=" + get_ID(response3.getBody()));
    request4.setField(Poco::Net::HTTPRequest::COOKIE, cookies);
    sf::Http::Response response4 = http.sendRequest(request4);

    // 5 get player token
    sf::Http::Request request5("/game");
    request5.setField(Poco::Net::HTTPRequest::COOKIE, cookies);
    sf::Http::Response response5 = http.sendRequest(request5);
    token = get_PLAYER_TOKEN(response5.getBody());
}

std::string Network::get_init()
{
    sf::Http http("fantasy-world.pl");
    sf::Http::Request request1("/game/init/" + token);
    request1.setField(Poco::Net::HTTPRequest::COOKIE, cookies);
    sf::Http::Response response1 = http.sendRequest(request1);
    return response1.getBody();
}

void Network::send(const std::string& json)
{
    socket.sendFrame(json.data(), json.size());
}

std::string Network::receive()
{
    int flags;
    socket.receiveFrame(buffer, flags);
    return buffer.begin();
}
