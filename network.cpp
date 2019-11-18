#include "network.hpp"
#include "resourcemanager.hpp"
#include <SFML/Network/Http.hpp>

namespace
{
std::string getCOOKIE(const std::string& body, size_t k)
{
    return body.substr(0, body.find(';') + k);
}
std::string getID(const std::string& body)
{
    size_t pos = body.find("value") + 7;
    return body.substr(pos, body.find('\"', pos) - pos);
}
std::string getPLAYER_TOKEN(const std::string& body)
{
    size_t pos = body.find("token") + 9;
    return body.substr(pos, body.find('\'', pos) - pos);
}
std::string getLOOKTYPE(const std::string& body)
{
    size_t pos = body.find("url") + 5;
    return body.substr(pos, body.find('\'', pos) - pos);
}
}

Network::Network():
    session("54.37.227.73", 9001),
    request(Poco::Net::HTTPRequest::HTTP_GET, "/echobot", Poco::Net::HTTPRequest::HTTP_1_1),
    buffer(0),
    socket(session, request, response) {}

void Network::login(const std::string& login, const std::string& password)
{
    sf::Http http("fantasy-world.pl");

    // 1. get first cookie
    sf::Http::Request request1("/modal/get/login");
    sf::Http::Response response1 = http.sendRequest(request1);
    cookies = getCOOKIE(response1.getField(Poco::Net::HTTPResponse::SET_COOKIE), 2);

    // 2. log in and get second cookie
    sf::Http::Request request2("/ajax/login", sf::Http::Request::Post, "login=" + login + "&password=" + password);
    request2.setField(Poco::Net::HTTPRequest::COOKIE, cookies);
    sf::Http::Response response2 = http.sendRequest(request2);
    cookies += getCOOKIE(response2.getField(Poco::Net::HTTPResponse::SET_COOKIE), 0);

    // 3. get hero id
    sf::Http::Request request3;
    request3.setField(Poco::Net::HTTPRequest::COOKIE, cookies);
    sf::Http::Response response3 = http.sendRequest(request3);

    // 4. set hero id
    sf::Http::Request request4("/game/login", sf::Http::Request::Post, "id=" + getID(response3.getBody()));
    request4.setField(Poco::Net::HTTPRequest::COOKIE, cookies);
    sf::Http::Response response4 = http.sendRequest(request4);

    // 5. get player token and looktype
    sf::Http::Request request5("/game");
    request5.setField(Poco::Net::HTTPRequest::COOKIE, cookies);
    sf::Http::Response response5 = http.sendRequest(request5);
    token = getPLAYER_TOKEN(response5.getBody());
    ResourceManager::setLooktype(getLOOKTYPE(response5.getBody()));
}

void Network::sendInit(sf::Vector2u windowSize)
{
    std::vector<Poco::DynamicAny> jsonArray;
    jsonArray.emplace_back(windowSize.x);
    jsonArray.emplace_back(windowSize.y);

    Poco::DynamicStruct json;
    json.insert("code", 1);
    json.insert("window", jsonArray);
    json.insert("token", token);

    send(json.toString());
}

Poco::DynamicStruct Network::receive()
{
    int flags;
    buffer.resize(0, false);
    socket.receiveFrame(buffer, flags);
    return Poco::DynamicAny::parse(std::string(buffer.begin(), buffer.size())).extract<Poco::DynamicStruct>();
}

Poco::DynamicStruct Network::receiveInit()
{
    sf::Http http("fantasy-world.pl");
    sf::Http::Request request1("/game/init/" + token);
    request1.setField(Poco::Net::HTTPRequest::COOKIE, cookies);
    sf::Http::Response response1 = http.sendRequest(request1);
    return Poco::DynamicAny::parse(response1.getBody()).extract<Poco::DynamicStruct>();
}

void Network::move(int dir)
{
    Poco::DynamicStruct data;
    data.insert("dir", dir);
    send(data, 5);
}

void Network::attack(int id)
{
    Poco::DynamicStruct data;
    data.insert("monster", id);
    data.insert("skill", 0);
    send(data, 3);
}

// private

void Network::send(const Poco::DynamicStruct& data, int code)
{
    Poco::DynamicStruct json;
    json.insert("code", code);
    json.insert("data", data);
    send(json.toString());
}

void Network::send(const std::string& json)
{
    socket.sendFrame(json.data(), json.size());
}
