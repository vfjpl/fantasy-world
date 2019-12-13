#include "network.hpp"
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
    request(Poco::Net::HTTPRequest::HTTP_GET,
            "/echobot",
            Poco::Net::HTTPRequest::HTTP_1_1),
    buffer(0),
    socket(http, request, response) {}

void Network::login(const std::string& login, const std::string& password)
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_POST,
                                "/ajax/login",
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    Poco::Net::HTMLForm html;
    html.add("login", login);
    html.add("password", password);
    html.prepareSubmit(requ);
    html.write(https.sendRequest(requ));
    https.receiveResponse(resp);

    std::vector<Poco::Net::HTTPCookie> cookies_vector;
    resp.getCookies(cookies_vector);
    for(auto &i: cookies_vector)
        cookies.add(i.getName(), i.getValue());
}

std::string Network::getListOfIDs()
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    return getID(https.receiveResponse(resp));
}

void Network::selectHero(const std::string& id)
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_POST,
                                "/game/login",
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    Poco::Net::HTMLForm html;
    requ.setCookies(cookies);
    html.add("id", id);
    html.prepareSubmit(requ);
    html.write(https.sendRequest(requ));
    https.receiveResponse(resp);
}

std::string Network::getLookType()
{
    sf::Http sfhttp("fantasy-world.pl");
    sf::Http::Request requ("/game");
    requ.setField(Poco::Net::HTTPRequest::COOKIE,
                  cookies.begin()->first + '=' +
                  cookies.begin()->second + ';' +
                  (--cookies.end())->first + '=' +
                  (--cookies.end())->second);
    sf::Http::Response resp = sfhttp.sendRequest(requ);
    token = getTOKEN(resp.getBody());
    return getLOOKTYPE(resp.getBody());
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

Poco::DynamicStruct Network::receiveInit()
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                "/game/init/" + token,
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    return Poco::DynamicAny::parse(toString(https.receiveResponse(resp))).extract<Poco::DynamicStruct>();
}

Poco::DynamicStruct Network::receive()
{
    int flags;
    buffer.resize(0, false);
    socket.receiveFrame(buffer, flags);
    return Poco::DynamicAny::parse(std::string(buffer.begin(), buffer.size())).extract<Poco::DynamicStruct>();
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
