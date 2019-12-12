#include "network.hpp"
#include "playerdata.hpp"
#include <Poco/Net/HTMLForm.h>
#include <SFML/Network/Http.hpp>

namespace
{
std::string toString(std::istream& stream)
{
    std::string body;
    std::getline(stream, body, '\0');
    return body;
}
std::string getID(std::istream& stream)
{
    std::string line;
    std::size_t pos;
    do
    {
        std::getline(stream, line);
        pos = line.find("value");
    }
    while(pos == std::string::npos);
    pos += 7;
    return line.substr(pos, line.find('"', pos) - pos);
}
std::string getTOKEN(const std::string& body)
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
    https("fantasy-world.pl",
          Poco::Net::HTTPSClientSession::HTTPS_PORT,
          new Poco::Net::Context(Poco::Net::Context::CLIENT_USE,
                                 std::string(),
                                 Poco::Net::Context::VERIFY_NONE)),
    http("54.37.227.73", 9001),
    request(Poco::Net::HTTPRequest::HTTP_GET, "/echobot", Poco::Net::HTTPRequest::HTTP_1_1),
    buffer(0),
    socket(http, request, response) {}

void Network::login(const std::string& login, const std::string& password)
{
    //1. login
    Poco::Net::HTTPRequest request1(Poco::Net::HTTPRequest::HTTP_POST, "/ajax/login");
    Poco::Net::HTTPResponse response1;
    Poco::Net::HTMLForm html1;
    html1.add("login", login);
    html1.add("password", password);
    html1.prepareSubmit(request1);
    html1.write(https.sendRequest(request1));
    https.receiveResponse(response1);

    //2. cookies
    std::vector<Poco::Net::HTTPCookie> cookies_vector;
    response1.getCookies(cookies_vector);
    for(auto &i: cookies_vector)
        cookies.add(i.getName(), i.getValue());

    //3. get id
    Poco::Net::HTTPRequest request2;
    Poco::Net::HTTPResponse response2;
    request2.setCookies(cookies);
    https.sendRequest(request2);
    std::string id = getID(https.receiveResponse(response2));

    //4. set id
    Poco::Net::HTTPRequest request3(Poco::Net::HTTPRequest::HTTP_POST, "/game/login");
    Poco::Net::HTTPResponse response3;
    Poco::Net::HTMLForm html3;
    request3.setCookies(cookies);
    html3.add("id", id);
    html3.prepareSubmit(request3);
    html3.write(https.sendRequest(request3));
    https.receiveResponse(response3);

    //5. token and looktype
    sf::Http http("fantasy-world.pl");
    sf::Http::Request request5("/game");
    request5.setField(Poco::Net::HTTPRequest::COOKIE,
                      cookies_vector.front().getName() + '=' +
                      cookies_vector.front().getValue() + ';' +
                      cookies_vector.back().getName() + '=' +
                      cookies_vector.back().getValue());
    sf::Http::Response response5 = http.sendRequest(request5);
    token = getTOKEN(response5.getBody());
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
    Poco::Net::HTTPRequest request1(Poco::Net::HTTPRequest::HTTP_GET, "/game/init/" + token);
    Poco::Net::HTTPResponse response1;
    request1.setCookies(cookies);
    https.sendRequest(request1);
    std::string body = toString(https.receiveResponse(response1));
    return Poco::DynamicAny::parse(body).extract<Poco::DynamicStruct>();
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
