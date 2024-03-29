#include "network.hpp"
#include "localplayer.hpp"
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/DynamicStruct.h>
#include <Poco/Net/HTMLForm.h>

//344
static Poco::Net::HTTPSClientSession https("alkatria.pl");
static Poco::Net::HTTPSClientSession wssHTTPS("alkatria.pl");
//136
static Poco::Net::HTTPRequest wssREQUEST;
//112
static Poco::Net::HTTPResponse wssRESPONSE;
//32
static Poco::Net::NameValueCollection cookies;
static Poco::Buffer<char> buffer(0ul);
//8
static Poco::Net::WebSocket* webSocket;


static std::string toString(std::istream& stream)
{
    std::string body;
    std::getline(stream, body, '\0');
    return body;
}

static void sendJson(const std::string& json)
{
    webSocket->sendFrame(json.data(), json.size());
}

static void send(const Poco::DynamicStruct& data, unsigned long code)
{
    Poco::DynamicStruct json;
    json.insert("code", code);
    json.insert("data", data);
    sendJson(json.toString());
}

static void send(const Poco::DynamicStruct& data, const char* code)
{
    Poco::DynamicStruct json;
    json.insert("code", code);
    json.insert("data", data);
    sendJson(json.toString());
}

static void sendStart(const std::string& token)
{
    std::vector<Poco::DynamicAny> jsonArray;
    jsonArray.emplace_back(1000ul);
    jsonArray.emplace_back(1000ul);

    Poco::DynamicStruct json;
    json.insert("code", 1ul);
    json.insert("window", jsonArray);
    json.insert("token", token);

    sendJson(json.toString());
}

static std::string getLOOKTYPE(const std::string& body)
{
    unsigned long pos = body.find("url") + 5ul;
    return body.substr(pos, body.find('\'', pos) - pos);
}

static std::string getTOKEN(const std::string& body)
{
    unsigned long pos = body.find("token") + 9ul;
    return body.substr(pos, body.find('\'', pos) - pos);
}

static std::string getURI(const std::string& body)
{
    unsigned long pos = body.find("host") + 9ul;
    return '/' + body.substr(pos, body.find('\'', pos) - pos);
}

static std::string loadGameData()
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                "/game",
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    return toString(https.receiveResponse(resp));
}

static tgui::ListBox::Ptr createHeroesListBox(const std::string& body)
{
    tgui::ListBox::Ptr listBox = tgui::ListBox::create();
    for(unsigned long start_pos = body.find("login");;)
    {
        start_pos += 6ul;
        unsigned long end_pos = body.find('\'', start_pos + 1ul);
        listBox->addItem(body.substr(start_pos, end_pos - start_pos));
        start_pos = body.find("login", end_pos + 1ul);
        if(start_pos == std::string::npos)
            break;
    }
    return listBox;
}

static bool isLoginSucessfull(const Poco::DynamicAny& data)
{
    bool check1 = data["code"] == 200ul;
    bool check2 = data["status"] == 200ul;
    return check1 && check2;
}


std::istream& Network::receiveData(const std::string& uri)
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                uri,
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    https.sendRequest(requ);
    return https.receiveResponse(resp);
}

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

void Network::startWebSocket()
{
    std::string body = loadGameData();
    wssREQUEST.setURI(getURI(body));
    webSocket = new Poco::Net::WebSocket(wssHTTPS, wssREQUEST, wssRESPONSE);
    sendStart(getTOKEN(body));
    LocalPlayer::looktype = getLOOKTYPE(body);
}

void Network::stopWebSocket()
{
    delete webSocket;
}

Poco::DynamicAny Network::receive(const std::string& token)
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                "/json.php?token=" + token,
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    return Poco::DynamicAny::parse(toString(https.receiveResponse(resp)));
}

Poco::DynamicAny Network::receive()
{
    int flags;
    buffer.resize(0ul, false);
    webSocket->receiveFrame(buffer, flags);
    return Poco::DynamicAny::parse(Poco::UTF8::unescape(
                                       std::string::const_iterator(buffer.begin()),
                                       std::string::const_iterator(buffer.end())));
}


void Network::attackMonster(unsigned long target_id)
{
    Poco::DynamicStruct data;
    data.insert("monster", target_id);
    data.insert("skill", 0ul);
    send(data, 3ul);
}

void Network::message(const sf::String& message)
{
    Poco::DynamicStruct data;
    data.insert("message", (const char*)message.toUtf8().data());
    send(data, 4ul);
}

void Network::move(unsigned long dir)
{
    Poco::DynamicStruct data;
    data.insert("dir", dir);
    send(data, 5ul);
}

void Network::use(unsigned long slot, const char* type)
{
    Poco::DynamicStruct data;
    data.insert("slot", slot);
    data.insert("type", type);
    send(data, 9ul);
}

void Network::takeLoot(unsigned long index)
{
    Poco::DynamicStruct data;
    data.insert("action", index);
    send(data, 18ul);
}

void Network::sendReloadPlayer(unsigned long player_id)
{
    Poco::DynamicStruct data;
    data.insert("player", player_id);
    send(data, 71ul);
}

void Network::pickUpItem(unsigned long x, unsigned long y)
{
    Poco::DynamicStruct data;
    data.insert("x", x);
    data.insert("y", y);
    send(data, 877ul);
}

void Network::useElement(unsigned long x, unsigned long y)
{
    Poco::DynamicStruct data;
    data.insert("x", x);
    data.insert("y", y);
    send(data, 879ul);
}

void Network::sendReload()
{
    Poco::DynamicStruct data;
    send(data, 1019ul);
}

void Network::attackPlayer(unsigned long target_id)
{
    Poco::DynamicStruct data;
    data.insert("target", target_id);
    data.insert("skill", 0ul);
    send(data, 1042ul);
}

void Network::openChest(unsigned long id)
{
    Poco::DynamicStruct data;
    data.insert("id", id);
    send(data, "chest");
}

void Network::spell(unsigned long spell_id)
{
    Poco::DynamicStruct data;
    data.insert("spell", spell_id);
    data.insert("type", 0ul);
    data.insert("fight_type", 0ul);
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
