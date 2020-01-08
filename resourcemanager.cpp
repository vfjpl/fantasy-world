#include "resourcemanager.hpp"
#include <Poco/Net/HTTPMessage.h>
#include <SFML/Network/Http.hpp>
#include <SFML/System/MemoryInputStream.hpp>

std::map<std::string, sf::Texture> ResourceManager::storage;

namespace
{
std::string getURI(const std::string& name, Graphic type)
{
    switch(type)
    {
    case MAP_DATA:
        return "/assets/" + name + ".png";
    case MAP:
        return "/assets/maps/files/" + name + ".png";
    case CHEST_OPEN:
        return "/templates/client/default/images/tiles/chest_" + name + "_open.png";
    case CHEST:
        return "/templates/client/default/images/tiles/chest_" + name + ".png";
    case ITEM:
        return "/templates/client/default/images/items/" + name + ".png";
    case MONSTER:
        return "/templates/client/default/images/monsters/" + name + ".gif";
    case NPC:
        return "/assets/avatars/" + name + ".gif";
    case PLAYER:
        return "/assets/looktypes/" + name + ".gif";
    default:
        return name;
    }//end switch
}
}

const sf::Texture& ResourceManager::getTexture(const std::string& name, Graphic type)
{
    return getTexture(getURI(name, type));
}

// private

const sf::Texture& ResourceManager::getTexture(const std::string& name)
{
    loadGraphic(name);

    return storage[name];
}

void ResourceManager::loadGraphic(const std::string& name)
{
    if(storage.count(name))
        return;

    sf::Http http("fantasy-world.pl");
    sf::Http::Request req(name);
    sf::Http::Response resp = http.sendRequest(req);

    sf::MemoryInputStream data;
    data.open(resp.getBody().data(), std::stoul(resp.getField(Poco::Net::HTTPMessage::CONTENT_LENGTH)));

    storage[name].loadFromStream(data);
}
