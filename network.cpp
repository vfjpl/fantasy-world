#include "network.hpp"
#include <Poco/Net/HTMLForm.h>

namespace
{
std::string toString(std::istream& stream)
{
    std::string body;
    std::getline(stream, body, '\0');
    return body;
}
bool isLoginSucessfull(const Poco::DynamicAny& data)
{
    bool check1 = data["code"] == 200ul;
    bool check2 = data["status"] == 200ul;
    return check1 && check2;
}
tgui::ListBox::Ptr createHeroesListBox(const std::string& body)
{
    tgui::ListBox::Ptr listBox = tgui::ListBox::create();
    for(unsigned long start_pos = body.find("login");;)
    {
        start_pos += 6;
        unsigned long end_pos = body.find('\'', start_pos + 1);
        listBox->addItem(body.substr(start_pos, end_pos - start_pos));
        start_pos = body.find("login", end_pos + 1);
        if(start_pos == std::string::npos)
            break;
    }
    return listBox;
}
std::string getLOOKTYPE(const std::string& body)
{
    unsigned long pos = body.find("url") + 5;
    return body.substr(pos, body.find('\'', pos) - pos);
}
std::string getTOKEN(const std::string& body)
{
    unsigned long pos = body.find("token") + 9;
    return body.substr(pos, body.find('\'', pos) - pos);
}
}

Network::Network():
    https("alkatria.pl"),
    wssHTTPS("alkatria.pl"),
    wssREQUEST(Poco::Net::HTTPRequest::HTTP_GET,
               "/websocket-test",
               Poco::Net::HTTPRequest::HTTP_1_1),
    buffer(0),
    socket(wssHTTPS, wssREQUEST, wssRESPONSE) {}


bool Network::credentials(const std::string& login, const std::string& password)
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
    if(!isLoginSucessfull(Poco::DynamicAny::parse(toString(https.receiveResponse(resp)))))
        return false;

    std::vector<Poco::Net::HTTPCookie> cookies_vector;
    resp.getCookies(cookies_vector);
    for(const auto& i: cookies_vector)
        cookies.add(i.getName(), i.getValue());

    return true;
}

tgui::ListBox::Ptr Network::getHeroesList()
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                "/modal/get/player-select",
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    return createHeroesListBox(toString(https.receiveResponse(resp)));
}

void Network::selectHero(const std::string& hero)
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                "/game/login/" + hero,
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    https.receiveResponse(resp);
}

std::string Network::getToken(LocalPlayer* localplayer)
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                "/game",
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    std::string body = toString(https.receiveResponse(resp));
    localplayer->looktype = getLOOKTYPE(body);
    return getTOKEN(body);
}

void Network::sendInit(const std::string& token, sf::Vector2u windowSize)
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

Poco::DynamicAny Network::receiveInit(const std::string& token)
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                "/game/init/" + token,
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    return Poco::DynamicAny::parse(toString(https.receiveResponse(resp)));
}

Poco::DynamicAny Network::receive()
{
    int flags;
    buffer.resize(0, false);
    socket.receiveFrame(buffer, flags);
    return Poco::DynamicAny::parse(Poco::UTF8::unescape(
                                       std::string::const_iterator(buffer.begin()),
                                       std::string::const_iterator(buffer.end())));
}

void Network::attackMonster(unsigned long target_id)
{
    Poco::DynamicStruct data;
    data.insert("monster", target_id);
    data.insert("skill", 0);
    send(data, 3);
}

void Network::message(const sf::String& message)
{
    Poco::DynamicStruct data;
    data.insert("message", (const char*)message.toUtf8().data());
    send(data, 4);
}

void Network::move(unsigned long dir)
{
    Poco::DynamicStruct data;
    data.insert("dir", dir);
    send(data, 5);
}

void Network::use(unsigned long slot, const char* type)
{
    Poco::DynamicStruct data;
    data.insert("slot", slot);
    data.insert("type", type);
    send(data, 9);
}

void Network::takeLoot()
{
    Poco::DynamicStruct data;
    data.insert("action", 0);
    send(data, 18);
}

void Network::pickUpItem(unsigned long x, unsigned long y)
{
    Poco::DynamicStruct data;
    data.insert("x", x);
    data.insert("y", y);
    send(data, 877);
}

void Network::useElement(unsigned long x, unsigned long y)
{
    Poco::DynamicStruct data;
    data.insert("x", x);
    data.insert("y", y);
    send(data, 879);
}

void Network::attackPlayer(unsigned long target_id)
{
    Poco::DynamicStruct data;
    data.insert("target", target_id);
    data.insert("skill", 0);
    send(data, 1042);
}

void Network::chest(unsigned long id)
{
    Poco::DynamicStruct data;
    data.insert("id", id);
    send(data, "chest");
}

void Network::spell(unsigned long spell_id)
{
    Poco::DynamicStruct data;
    data.insert("spell", spell_id);
    data.insert("type", 0);
    data.insert("fight_type", 0);
    send(data, "spell");
}

void Network::spellMonster(unsigned long spell_id, unsigned long target_id)
{
    Poco::DynamicStruct data;
    data.insert("spell", spell_id);
    data.insert("target", target_id);
    data.insert("fight_type", "monster");
    send(data, "spell");
}

// private

void Network::send(const Poco::DynamicStruct& data, unsigned long code)
{
    Poco::DynamicStruct json;
    json.insert("code", code);
    json.insert("data", data);
    send(json.toString());
}

void Network::send(const Poco::DynamicStruct& data, const char* code)
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
