#include "resourcemanager.hpp"
#include <Poco/Net/HTTPMessage.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <SFML/Network/Http.hpp>
#include <SFML/System/FileInputStream.hpp>
#include <SFML/System/MemoryInputStream.hpp>

namespace
{
//48
std::map<std::string, sf::Texture> storage;

void loadGraphic(const std::string& URI)
{
    if(storage.count(URI))
        return;

    std::string path = "cache" + URI;
    sf::FileInputStream sfFile;
    if(sfFile.open(path))
    {
        storage[URI].loadFromStream(sfFile);
        return;
    }

    sf::Http http("alkatria.pl");
    sf::Http::Request requ(URI);
    sf::Http::Response resp = http.sendRequest(requ);
    const std::string& body = resp.getBody();

    sf::MemoryInputStream data;
    data.open(body.data(), std::stoul(resp.getField(Poco::Net::HTTPMessage::CONTENT_LENGTH)));
    storage[URI].loadFromStream(data);

    Poco::File dir(path.substr(0, path.rfind('/')));
    dir.createDirectories();
    Poco::FileOutputStream pocoFile(path);
    pocoFile << body;
}

const sf::Texture& getTextureByURI(const std::string& URI)
{
    loadGraphic(URI);
    return storage[URI];
}

std::string getURI(const std::string& name, Graphic type)
{
    switch(type)
    {
    case Graphic::NPC:
        return "/assets/avatars/" + name + ".gif";
    case Graphic::PLAYER:
        return "/assets/looktypes/" + name + ".gif";
    case Graphic::ATTACK_EFFECT:
        return "/assets/attacks/" + name + ".gif";
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
}//end namespace


const sf::Texture& ResourceManager::getTexture(const std::string& name, Graphic type)
{
    return getTextureByURI(getURI(name, type));
}
