#include "resourcemanager.hpp"
#include <Poco/Net/HTTPMessage.h>
#include <SFML/Network/Http.hpp>
#include <SFML/System/Thread.hpp>
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
std::set<std::string> getNAMES(const Poco::DynamicStruct& data)
{
    std::set<std::string> names;

    if(data.contains("map_data"))
    {
        auto& map_data = data["map_data"];
        for(sf::Uint8 i = 0; i < map_data.size(); ++i)
            names.emplace(getURI(map_data[i]["source"], MAP_DATA));
    }
    else
    {
        names.emplace(getURI(data["map"]["id"], MAP));
    }

    if(data.contains("map_items"))
    {
        auto& map_items = data["map_items"];
        for(sf::Uint8 i = 0; i < map_items.size(); ++i)
            names.emplace(getURI(map_items[i]["item_id"], ITEM));
    }

    auto& monsters = data["monsters"];
    for(sf::Uint8 i = 0; i < monsters.size(); ++i)
        names.emplace(getURI(monsters[i]["looktype"], MONSTER));

    auto& npcs = data["npcs"];
    for(sf::Uint8 i = 0; i < npcs.size(); ++i)
        names.emplace(getURI(npcs[i]["looktype"], NPC));

    auto& players = data["players"];
    for(sf::Uint8 i = 0; i < players.size(); ++i)
        names.emplace(getURI(players[i]["looktype"], PLAYER));

    return names;
}
}

const sf::Texture& ResourceManager::getTexture(const std::string& name, Graphic type)
{
    return getTexture(getURI(name, type));
}

void ResourceManager::loadParallel(const Poco::DynamicStruct& data)
{
    loadParallel(getNAMES(data));
}

// private

const sf::Texture& ResourceManager::getTexture(const std::string& name)
{
    loadGraphic(name);

    return storage[name];
}

void ResourceManager::loadParallel(const std::set<std::string>& names)
{
    std::vector<sf::Thread*> threads;

    for(auto &i: names)
    {
        //threads.emplace_back(new sf::Thread());
        threads.back()->launch();
    }

    for(auto &i: threads)
        delete i;
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

    if(!storage[name].loadFromStream(data))
    {
        //RESIZING
        sf::Image orginal;
        sf::Image resized;
        orginal.loadFromStream(data);

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
