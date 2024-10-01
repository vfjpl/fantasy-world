#include "network.hpp"
#include "localplayer.hpp"
#include "helperfunctions.hpp"
#include <Poco/Net/HTMLForm.h>

Network_t Network;


static std::string toString(std::istream& stream)
{
    std::string body;
    std::getline(stream, body, '\0');
    return body;
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

static tgui::ListBox::Ptr createHeroesListBox(const std::string& body)
{
	tgui::ListBox::Ptr listBox = tgui::ListBox::create();
	for(unsigned long start_pos = body.find("login");;)
	{
		start_pos += 6ul;
		unsigned long end_pos = body.find('\'', start_pos + 1ul);
		listBox->addItem(fromUtf8(body.cbegin() + start_pos, body.cbegin() + end_pos));
		start_pos = body.find("login", end_pos + 1ul);
		if(start_pos == std::string::npos)
			break;
	}
	return listBox;
}

static bool isLoginSucessfull(const Poco::DynamicAny& data)
{
	bool check1 = data["code"] == 200l;
	bool check2 = data["status"] == 200l;
	return check1 & check2;
}


void Network_t::sendJson(const std::string& json)
{
    webSocket->sendFrame(json.data(), json.size());
}

void Network_t::send(const Poco::DynamicStruct& data, long code)
{
    Poco::DynamicStruct json;
    json.insert("code", code);
    json.insert("data", data);
    sendJson(json.toString());
}

void Network_t::send(const Poco::DynamicStruct& data, const char* code)
{
    Poco::DynamicStruct json;
    json.insert("code", code);
    json.insert("data", data);
    sendJson(json.toString());
}

void Network_t::sendStart(const std::string& token)
{
    std::vector<Poco::DynamicAny> jsonArray;
    jsonArray.emplace_back(1000l);
    jsonArray.emplace_back(1000l);

    Poco::DynamicStruct json;
    json.insert("code", 1l);
    json.insert("window", jsonArray);
    json.insert("token", token);

    sendJson(json.toString());
}

std::string Network_t::loadGameData()
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                "/game",
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    return toString(https.receiveResponse(resp));
}


Network_t::Network_t():
	https("alkatria.pl"),
	wssHTTPS("alkatria.pl"),
	wssREQUEST(Poco::Net::HTTPRequest::HTTP_1_1),
	buffer(0ul) {}


std::istream& Network_t::receiveData(const std::string& uri)
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                uri,
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    https.sendRequest(requ);
    return https.receiveResponse(resp);
}


bool Network_t::credentials(const std::string& login, const std::string& password)
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
	for(const auto& i : cookies_vector)
		cookies.add(i.getName(), i.getValue());

	return true;
}

tgui::ListBox::Ptr Network_t::getHeroesList()
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                "/modal/get/player-select",
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    return createHeroesListBox(toString(https.receiveResponse(resp)));
}

void Network_t::selectHero(const std::string& hero)
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                "/game/login/" + hero,
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    https.receiveResponse(resp);
}

void Network_t::startWebSocket()
{
	std::string body = loadGameData();
	wssREQUEST.setURI(getURI(body));
	webSocket = std::make_unique<Poco::Net::WebSocket>(wssHTTPS, wssREQUEST, wssRESPONSE);
	sendStart(getTOKEN(body));
	LocalPlayer.looktype = getLOOKTYPE(body);
}

void Network_t::stopWebSocket()
{
	webSocket.reset();
}


Poco::DynamicAny Network_t::receive(const std::string& token)
{
    Poco::Net::HTTPRequest requ(Poco::Net::HTTPRequest::HTTP_GET,
                                "/json.php?token=" + token,
                                Poco::Net::HTTPRequest::HTTP_1_1);
    Poco::Net::HTTPResponse resp;
    requ.setCookies(cookies);
    https.sendRequest(requ);
    return Poco::DynamicAny::parse(toString(https.receiveResponse(resp)));
}

Poco::DynamicAny Network_t::receive()
{
	int flags = 0;
	buffer.resize(0ul, false);
	webSocket->receiveFrame(buffer, flags);
	//sometimes "multi_code" message needs second unescape before parse
	return Poco::DynamicAny::parse(Poco::UTF8::unescape(Poco::UTF8::unescape(
									std::string::const_iterator(buffer.begin()),
									std::string::const_iterator(buffer.end()))));
}


void Network_t::attackMonster(long target_id)
{
    Poco::DynamicStruct data;
    data.insert("monster", target_id);
    data.insert("skill", 0l);
    send(data, 3l);
}

void Network_t::message(const sf::String& message)
{
    Poco::DynamicStruct data;
    data.insert("message", (const char*)message.toUtf8().data());
    send(data, 4l);
}

void Network_t::move(long dir)
{
    Poco::DynamicStruct data;
    data.insert("dir", dir);
    send(data, 5l);
}

void Network_t::use(long slot, const char* type)
{
    Poco::DynamicStruct data;
    data.insert("slot", slot);
    data.insert("type", type);
    send(data, 9l);
}

void Network_t::takeLoot(long index)
{
    Poco::DynamicStruct data;
    data.insert("action", index);
    send(data, 18l);
}

void Network_t::sendReloadPlayer(long player_id)
{
    Poco::DynamicStruct data;
    data.insert("player", player_id);
    send(data, 71l);
}

void Network_t::pickUpItem(long x, long y)
{
    Poco::DynamicStruct data;
    data.insert("x", x);
    data.insert("y", y);
    send(data, 877l);
}

void Network_t::useElement(long x, long y)
{
    Poco::DynamicStruct data;
    data.insert("x", x);
    data.insert("y", y);
    send(data, 879l);
}

void Network_t::sendReload()
{
    Poco::DynamicStruct data;
    send(data, 1019l);
}

void Network_t::attackPlayer(long target_id)
{
    Poco::DynamicStruct data;
    data.insert("target", target_id);
    data.insert("skill", 0l);
    send(data, 1042l);
}

void Network_t::openChest(long id)
{
    Poco::DynamicStruct data;
    data.insert("id", id);
    send(data, "chest");
}

void Network_t::spell(long spell_id)
{
    Poco::DynamicStruct data;
    data.insert("spell", spell_id);
    data.insert("type", 0l);
    data.insert("fight_type", 0l);
    send(data, "spell");
}

void Network_t::spellMonster(long spell_id, long target_id)
{
    Poco::DynamicStruct data;
    data.insert("spell", spell_id);
    data.insert("target", target_id);
    data.insert("fight_type", "monster");
    send(data, "spell");
}
