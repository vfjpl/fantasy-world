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
bool isLoginSucessfull(const Poco::DynamicStruct& data)
{
    bool check1 = data["code"] == 200;
    bool check2 = data["status"] == 200;
    return check1 && check2;
}
std::vector<std::string> getIDs(const std::string& body)
{
    std::vector<std::string> IDs;
    for(size_t start_pos = body.find("value");;)
    {
        start_pos += 7;
        size_t end_pos = body.find('"', start_pos);
        IDs.emplace_back(body.substr(start_pos, end_pos - start_pos));
        start_pos = body.find("value", end_pos + 1);
        if(start_pos == std::string::npos)
            break;
    }
    return IDs;
}
std::string cookiesToString(const Poco::Net::NameValueCollection& collection)
{
    std::string cookies;
    for(auto& i: collection)
    {
        cookies.append(i.first);
        cookies.push_back('=');
        cookies.append(i.second);
        cookies.push_back(';');
    }
    return cookies;
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
    https("alkatria.pl"),
    http("54.37.227.73", 9001),
    request(Poco::Net::HTTPRequest::HTTP_1_1),
    buffer(0),
    socket(http, request, response) {}


bool Network::login1_credentials(const std::string& login, const std::string& password)
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
    if(!isLoginSucessfull(Poco::DynamicAny::parse(toString(https.receiveResponse(resp)))
                          .extract<Poco::DynamicStruct>()))
        return false;

    std::vector<Poco::Net::HTTPCookie> cookies_vector;
    resp.getCookies(cookies_vector);
    for(auto& i: cookies_vector)
        cookies.add(i.getName(), i.getValue());

    return true;
}

std::vector<std::string> Network::login2_getListOfIDs()
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    return getIDs(toString(https.receiveResponse(resp)));
}

void Network::login3_selectHero(const std::string& hero_id)
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_POST,
                                "/game/login",
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    Poco::Net::HTMLForm html;
    requ.setCookies(cookies);
    html.add("id", hero_id);
    html.prepareSubmit(requ);
    html.write(https.sendRequest(requ));
    https.receiveResponse(resp);
}

std::string Network::login4_getLookType()
{
    sf::Http sfhttp("alkatria.pl");
    sf::Http::Request sfrequ("/game");
    sfrequ.setField(Poco::Net::HTTPRequest::COOKIE, cookiesToString(cookies));
    sf::Http::Response sfresp = sfhttp.sendRequest(sfrequ);
    token = getTOKEN(sfresp.getBody());
    return getLOOKTYPE(sfresp.getBody());
}

void Network::login5_sendInit(sf::Vector2u windowSize)
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
