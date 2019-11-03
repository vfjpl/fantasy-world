#include "resource_manager.hpp"
#include <SFML/Network/Http.hpp>
#include <Poco/Net/HTTPMessage.h>

namespace
{
std::string getURI(const std::string& name, Graphic type)
{
    switch(type)
    {
    case MAP_TILE:
        return "/assets/" + name + ".png";
    case MAP_SINGLE:
        return "/assets/maps/files/" + name + ".png";
    case MONSTER:
        return "/templates/client/default/images/monsters/" + name + ".gif";
    case NPC:
        return "/assets/avatars/" + name + ".gif";
    case PLAYER:
        return "/assets/looktypes/" + name + ".gif";
    case DIRECT:
        return name;
    default:
        return std::string();
    }//end switch
}
}

const sf::Texture& Resource_Manager::getTexture(const std::string& name, Graphic type)
{
    return storage[getURI(name, type)];
}

void Resource_Manager::loadGraphic(const std::string& name, Graphic type)
{
    std::string uri = getURI(name, type);

    if(storage.count(uri))
        return;

    sf::Http http("fantasy-world.pl");
    sf::Http::Request req(uri);
    sf::Http::Response resp = http.sendRequest(req);

    unsigned long lenght = std::stoul(resp.getField(Poco::Net::HTTPMessage::CONTENT_LENGTH));
    size_in_bytes += lenght;

    if(!storage[uri].loadFromMemory(resp.getBody().data(), lenght))
    {
        //RESIZING
        sf::Image orginal;
        sf::Image resized;
        orginal.loadFromMemory(resp.getBody().data(), lenght);

        float scale = sf::Texture::getMaximumSize();
        sf::Vector2u orginal_size = orginal.getSize();
        sf::Vector2f scale_xy(orginal_size);
        scale_xy /= scale;
        if(scale_xy.x > scale_xy.y)
            scale = scale_xy.x;
        else
            scale = scale_xy.y;

        resized.create(orginal_size.x / scale, orginal_size.y / scale);
        sf::Vector2u resized_size = resized.getSize();

        for(unsigned int y = 0; y < resized_size.y; ++y)
            for(unsigned int x = 0; x < resized_size.x; ++x)
                resized.setPixel(x, y, orginal.getPixel(x * scale, y * scale));

        storage[uri].loadFromImage(resized);
    }
}
