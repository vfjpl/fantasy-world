#include "network.hpp"
#include "playerdata.hpp"
#include <SFML/Network/Http.hpp>

namespace
{
std::string getCOOKIE0(const std::string& body)
{
    return body.substr(0, body.find(';'));
}
std::string getCOOKIE2(const std::string& body)
{
    return body.substr(0, body.find(';') + 2);
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
    cookies = getCOOKIE2(response1.getField(Poco::Net::HTTPResponse::SET_COOKIE));

    // 2. log in and get second cookie
    sf::Http::Request request2("/ajax/login", sf::Http::Request::Post, "login=" + login + "&password=" + password);
    request2.setField(Poco::Net::HTTPRequest::COOKIE, cookies);
    sf::Http::Response response2 = http.sendRequest(request2);
    cookies += getCOOKIE0(response2.getField(Poco::Net::HTTPResponse::SET_COOKIE));

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
    PlayerData::looktype = getLOOKTYPE(response5.getBody());
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

void Network::attack(int id)
{
    Poco::DynamicStruct data;
    data.insert("monster", id);
    data.insert("skill", 0);

    Poco::DynamicStruct json;
    json.insert("code", 3);
    json.insert("data", data);
    send(json.toString());
}

void Network::message(const std::string& message)
{
    Poco::DynamicStruct data;
    data.insert("message", message);

    Poco::DynamicStruct json;
    json.insert("code", 4);
    json.insert("data", data);
    send(json.toString());
}

void Network::move(int dir)
{
    Poco::DynamicStruct data;
    data.insert("dir", dir);

    Poco::DynamicStruct json;
    json.insert("code", 5);
    json.insert("data", data);
    send(json.toString());
}

// private

void Network::send(const std::string& json)
{
    socket.sendFrame(json.data(), json.size());
}
