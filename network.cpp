#include "network.hpp"
#include <SFML/Network/Http.hpp>

namespace
{
std::string get_ID(const std::string& body)
{
    size_t pos = body.find("value") + 7;
    return body.substr(pos, body.find('\"', pos) - pos);
}
std::string get_PLAYER_TOKEN(const std::string& body)
{
    size_t pos = body.find("player_token") + 16;
    return body.substr(pos, body.find('\'', pos) - pos);
}
}

std::string Network::login(const std::string& login, const std::string& password)
{
    sf::Http http("fantasy-world.pl");

    // 1
    sf::Http::Request request1("/modal/get/login");
    sf::Http::Response response1 = http.sendRequest(request1);

    // 2
    sf::Http::Request request2("/ajax/login", sf::Http::Request::Post, "login=" + login + "&password=" + password);
    request2.setField("Cookie", response1.getField("Set-Cookie"));
    sf::Http::Response response2 = http.sendRequest(request2);

    // 3
    sf::Http::Request request3;
    request3.setField("Cookie", response1.getField("Set-Cookie") + "; " + response2.getField("Set-Cookie"));
    sf::Http::Response response3 = http.sendRequest(request3);

    // 4
    sf::Http::Request request4("/game/login", sf::Http::Request::Post, "id=" + get_ID(response3.getBody()));
    request4.setField("Cookie", response1.getField("Set-Cookie") + "; " + response2.getField("Set-Cookie"));
    sf::Http::Response response4 = http.sendRequest(request4);

    // 5
    sf::Http::Request request5("/game");
    request5.setField("Cookie", response1.getField("Set-Cookie") + "; " + response2.getField("Set-Cookie"));
    sf::Http::Response response5 = http.sendRequest(request5);

    return get_PLAYER_TOKEN(response5.getBody());
}
