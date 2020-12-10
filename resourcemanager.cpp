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
    case Graphic::NPC:
        return "/assets/avatars/" + name + ".gif";
    case Graphic::PLAYER:
        return "/assets/looktypes/" + name + ".gif";
    case Graphic::MONSTER:
        return "/templates/client/default/images/monsters/" + name + ".gif";
    case Graphic::MAP_MULTI:
        return "/assets/" + name + ".png";
    case Graphic::MAP_SINGLE:
        return "/assets/maps/files/" + name + ".png";
    case Graphic::ITEM:
        return "/templates/client/default/images/items/" + name + ".png";
    case Graphic::CHEST:
        return "/templates/client/default/images/tiles/chest_" + name + ".png";
    case Graphic::CHEST_OPEN:
        return "/templates/client/default/images/tiles/chest_" + name + "_open.png";
    case Graphic::GAME_OBJECT:
        return "/assets/game_objects/" + name;
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

    sf::Http http("alkatria.pl");
    sf::Http::Request requ(name);
    sf::Http::Response resp = http.sendRequest(requ);

    sf::MemoryInputStream data;
    data.open(resp.getBody().data(), std::stoul(resp.getField(Poco::Net::HTTPMessage::CONTENT_LENGTH)));

    storage[name].loadFromStream(data);
}
