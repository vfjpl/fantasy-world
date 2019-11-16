#include "resource_manager.hpp"
#include <Poco/Net/HTTPMessage.h>
#include <SFML/Network/Http.hpp>
#include <SFML/System/Thread.hpp>

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
    default:
        return name;
    }//end switch
}
std::set<std::string> getNAMES(const Poco::DynamicStruct& data)
{
    std::set<std::string> names;

    auto& monsters = data["monsters"];
    for(sf::Uint8 i = 0; i < monsters.size(); ++i)
    {
        auto& monster = monsters[i];
        names.emplace(getURI(monster["looktype"], MONSTER));
    }
    auto& npcs = data["npcs"];
    for(sf::Uint8 i = 0; i < npcs.size(); ++i)
    {
        auto& npc = npcs[i];
        names.emplace(getURI(npc["looktype"], NPC));
    }
    auto& players = data["players"];
    for(sf::Uint8 i = 0; i < players.size(); ++i)
    {
        auto& player = players[i];
        names.emplace(getURI(player["looktype"], PLAYER));
    }

    return names;
}
}

const sf::Texture& Resource_Manager::getTexture(const std::string& name, Graphic type)
{
    return getTexture(getURI(name, type));
}

const sf::Texture& Resource_Manager::getTexture(const std::string& name)
{
    loadGraphic(name);

    return storage[name];
}

void Resource_Manager::loadParallel(const Poco::DynamicStruct& data)
{
    loadParallel(getNAMES(data));
}

void Resource_Manager::loadParallel(const std::set<std::string>& names)
{
    std::vector<sf::Thread*> threads;

    for(auto &i: names)
    {
        threads.emplace_back(new sf::Thread([&]
        {
            loadGraphic(i);
        }));
        threads.back()->launch();
    }

    for(auto &i: threads)
        delete i;
}

void Resource_Manager::loadGraphic(const std::string& name)
{
    if(storage.count(name))
        return;

    sf::Http http("fantasy-world.pl");
    sf::Http::Request req(name);
    sf::Http::Response resp = http.sendRequest(req);

    unsigned long lenght = std::stoul(resp.getField(Poco::Net::HTTPMessage::CONTENT_LENGTH));

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
