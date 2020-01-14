#include "network.hpp"
#include <Poco/Net/HTMLForm.h>
#include <SFML/Network/Http.hpp>

namespace
{
std::vector<std::string> getIDs(std::istream& stream)
{
    std::string line;
    std::vector<std::string> IDs;
    size_t pos;

    do
    {
        std::getline(stream, line);
        pos = line.find("value");
    }
    while(pos == std::string::npos);

    do
    {
        pos += 7;
        size_t temp = line.find('"', pos);
        IDs.emplace_back(line.substr(pos, temp - pos));
        pos = line.find("value", temp + 1);
    }
    while(pos != std::string::npos);

    return IDs;
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
std::string toString(std::istream& stream)
{
    std::string body;
    std::getline(stream, body, '\0');
    return body;
}
bool isLoginSucessfull(const Poco::DynamicStruct& data)
{
    bool check1 = data["code"] == 200;
    bool check2 = data["status"] == 200;
    return check1 && check2;
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


bool Network::login(const std::string& login, const std::string& password)
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_POST,
                                "/ajax/login",
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    Poco::Net::HTMLForm html;
    requ.setCookies(cookies);
    html.add("login", login);
    html.add("password", password);
    html.prepareSubmit(requ);
    html.write(https.sendRequest(requ));
    bool ok = isLoginSucessfull(Poco::DynamicAny::parse(toString(https.receiveResponse(resp)))
                                .extract<Poco::DynamicStruct>());

    std::vector<Poco::Net::HTTPCookie> cookies_vector;
    resp.getCookies(cookies_vector);
    for(auto& i: cookies_vector)
        cookies.add(i.getName(), i.getValue());

    return ok;
}

std::vector<std::string> Network::getListOfIDs()
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    return getIDs(https.receiveResponse(resp));
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
    sf::Http::Request request("/game");
    request.setField(Poco::Net::HTTPRequest::COOKIE,
                     cookies.begin()->first + '=' +
                     cookies.begin()->second + ';' +
                     (--cookies.end())->first + '=' +
                     (--cookies.end())->second);
    sf::Http::Response response = sfhttp.sendRequest(request);
    token = getTOKEN(response.getBody());
    return getLOOKTYPE(response.getBody());
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
    return Poco::DynamicAny::parse(
               Poco::UTF8::unescape(
                   std::string::const_iterator(buffer.begin()),
                   std::string::const_iterator(buffer.end())))
           .extract<Poco::DynamicStruct>();
}

void Network::attackMonster(int id)
{
    Poco::DynamicStruct data;
    data.insert("monster", id);
    data.insert("skill", 0);
    send(data, 3);
}

void Network::message(const char* message)
{
    Poco::DynamicStruct data;
    data.insert("message", message);
    send(data, 4);
}

void Network::move(int dir)
{
    Poco::DynamicStruct data;
    data.insert("dir", dir);
    send(data, 5);
}

void Network::shortcut(int nr)
{
    Poco::DynamicStruct data;
    data.insert("slot", nr);
    data.insert("type", "shortcut");
    send(data, 9);
}

void Network::takeLoot()
{
    Poco::DynamicStruct data;
    data.insert("action", 0);
    send(data, 18);
}

void Network::attackPlayer(int id)
{
    Poco::DynamicStruct data;
    data.insert("target", id);
    data.insert("skill", 0);
    send(data, 1042);
}

void Network::spell(int nr)
{
    Poco::DynamicStruct data;
    data.insert("spell", nr);
    data.insert("type", 0);
    data.insert("fight_type", 0);

    Poco::DynamicStruct json;
    json.insert("code", "spell");
    json.insert("data", data);
    send(json.toString());
}

void Network::spell(int nr, int id)
{
    Poco::DynamicStruct data;
    data.insert("spell", nr);
    data.insert("target", id);
    data.insert("fight_type", "monster");

    Poco::DynamicStruct json;
    json.insert("code", "spell");
    json.insert("data", data);
    send(json.toString());
}

// private

void Network::send(const Poco::DynamicStruct& data, int nr)
{
    Poco::DynamicStruct json;
    json.insert("code", nr);
    json.insert("data", data);
    send(json.toString());
}

void Network::send(const std::string& json)
{
    socket.sendFrame(json.data(), json.size());
}
