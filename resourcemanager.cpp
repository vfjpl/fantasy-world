#include "resourcemanager.hpp"
#include <Poco/Net/HTTPMessage.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <SFML/Network/Http.hpp>
#include <SFML/System/FileInputStream.hpp>
#include <SFML/System/MemoryInputStream.hpp>

//48
static std::map<std::string, sf::Texture> storage;


static void loadGraphic(const std::string& path)
{
    if(storage.count(path))
        return;

    if(sf::FileInputStream sfFile; sfFile.open(path))
    {
        storage[path].loadFromStream(sfFile);
        return;
    }

    sf::Http http("alkatria.pl");
    sf::Http::Request requ(path.substr(path.find('/')));
    sf::Http::Response resp = http.sendRequest(requ);
    const std::string& body = resp.getBody();

    sf::MemoryInputStream data;
    data.open(body.data(), std::stoul(resp.getField(Poco::Net::HTTPMessage::CONTENT_LENGTH)));
    storage[path].loadFromStream(data);

    Poco::File dir(path.substr(0ul, path.rfind('/')));
    dir.createDirectories();

    Poco::FileOutputStream pocoFile(path);
    pocoFile << body;
}

static const sf::Texture& getTextureByPath(const std::string& path)
{
    loadGraphic(path);
    return storage[path];
}

static std::string getPath(const std::string& name, Graphic type)
{
    switch(type)
    {
    case Graphic::ATTACK_EFFECT:
        return "cache/assets/attacks/" + name + ".gif";
    case Graphic::NPC:
        return "cache/assets/avatars/" + name + ".gif";
    case Graphic::PLAYER:
        return "cache/assets/looktypes/" + name + ".gif";
    case Graphic::MONSTER:
        return "cache/templates/client/default/images/monsters/" + name + ".gif";
    case Graphic::MAP_MULTI:
        return "cache/assets/" + name + ".png";
    case Graphic::MAP_SINGLE:
        return "cache/assets/maps/files/" + name + ".png";
    case Graphic::ITEM:
        return "cache/templates/client/default/images/items/" + name + ".png";
    case Graphic::CHEST:
        return "cache/templates/client/default/images/tiles/chest_" + name + ".png";
    case Graphic::CHEST_OPEN:
        return "cache/templates/client/default/images/tiles/chest_" + name + "_open.png";
    case Graphic::GAME_OBJECT:
        return "cache/assets/game_objects/" + name;
    default:
        return "cache" + name;
    }//end switch
}


const sf::Texture& ResourceManager::getTexture(const std::string& name, Graphic type)
{
    return getTextureByPath(getPath(name, type));
}
