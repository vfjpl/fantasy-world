#include "resource_manager.hpp"
#include <SFML/Network/Http.hpp>
#include <Poco/Net/HTTPMessage.h>

const sf::Texture& Resource_Manager::get_texture(const std::string& name)
{
    return storage[name];
}

void Resource_Manager::load_graphic(const std::string& name, Graphic type)
{
    if(storage.count(name))
        return;

    std::string uri;
    switch(type)
    {
    case MAP_TILE:
        uri = "/assets/" + name + ".png";
        break;
    case MAP_SINGLE:
        uri = "/assets/maps/files/" + name + ".png";
        break;
    case MONSTER:
        uri = "/templates/client/default/images/monsters/" + name + ".gif";
        break;
    case NPC:
        uri = "/assets/avatars/" + name + ".gif";
        break;
    case PLAYER:
        uri = "/assets/looktypes/" + name + ".gif";
        break;
    case DIRECT:
        uri = name;
        break;
    }//end switch

    sf::Http http("fantasy-world.pl");
    sf::Http::Request req(uri);
    sf::Http::Response resp = http.sendRequest(req);

    unsigned long lenght = std::stoul(resp.getField(Poco::Net::HTTPMessage::CONTENT_LENGTH));
    size_in_bytes += lenght;

    if(!storage[name].loadFromMemory(resp.getBody().data(), lenght))
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

        storage[name].loadFromImage(resized);
    }
}
