#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/DynamicStruct.h>
#include <Poco/DynamicAny.h>
#include <TGUI/Widgets/ListBox.hpp>

class Network_t
{
	//344
	Poco::Net::HTTPSClientSession https;
	Poco::Net::HTTPSClientSession wssHTTPS;
	//136
	Poco::Net::HTTPRequest wssREQUEST;
	//112
	Poco::Net::HTTPResponse wssRESPONSE;
	//32
	Poco::Net::NameValueCollection cookies;
	Poco::Buffer<char> buffer;
	//8
	std::unique_ptr<Poco::Net::WebSocket> webSocket;


	void sendJson(const std::string& json);
	void send(const Poco::DynamicStruct& data, long code);
	void send(const Poco::DynamicStruct& data, const char* code);
	void sendStart(const std::string& token);
	std::string loadGameData();

public:
	Network_t();

	std::istream& receiveData(const std::string& uri);

	bool credentials(const std::string& login, const std::string& password);
	tgui::ListBox::Ptr getHeroesList();
	void selectHero(const std::string& hero);
	void startWebSocket();
	void stopWebSocket();

	Poco::DynamicAny receive(const std::string& token);
	Poco::DynamicAny receive();

	void attackMonster(long target_id);
	void message(const sf::String& message);
	void move(long dir);
	void use(long slot, const char* type);
	void takeLoot(long index);
	void sendReloadPlayer(long player_id);
	void pickUpItem(long x, long y);
	void useElement(long x, long y);
	void sendReload();
	void attackPlayer(long target_id);
	void openChest(long id);
	void spell(long spell_id);
	void spellMonster(long spell_id, long target_id);
};

extern Network_t Network;

#endif // NETWORK_HPP_INCLUDED
