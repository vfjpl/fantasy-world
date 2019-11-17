#include "engine.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>

//view-source:http://fantasy-world.pl/templates/client/default/js/game.js

namespace
{
constexpr int char2int(const char* str)
{
    int res = 0;
    for(sf::Uint8 i = 0; i < strlen(str); ++i)
        res -= str[i];
    return res;
}
int str2int(const std::string& str)
{
    int res = 0;
    for(sf::Uint8 i = 0; i < str.size(); ++i)
        res -= str[i];
    return res;
}
int var2int(const Poco::DynamicAny& var)
{
    if(var.isInteger())
        return var;
    return str2int(var);
}
}

void Engine::setup()
{
    setup_window(true);
    network.login();
    network.sendInit(window.getSize());
    map.player.setTexture(resourceManager.getTexture(network.getPlayerLooktype()));
}

bool Engine::run_game()
{
    process_input();
    draw_frame();

    return window.isOpen();
}

bool Engine::run_network()
{
    process_network(network.receive());

    return window.isOpen();
}

// private

void Engine::setup_window(bool fullscreen)
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    if(fullscreen)
    {
        window.create(mode, "fantasy-world", sf::Style::Fullscreen);
    }
    else
    {
        mode.width = (mode.width*2)/3;
        mode.height = (mode.height*2)/3;
        window.create(mode, "fantasy-world", sf::Style::Close);
    }
    window.setKeyRepeatEnabled(false);
    camera = window.getDefaultView();
}

void Engine::cameraCenterInstant(int x, int y)
{
    cameraCenterSmooth(x, y);
    camera.setCenter(center.x, center.y);
}

void Engine::cameraCenterSmooth(int x, int y)
{
    center.x = (32 * x) - 16;
    center.y = (32 * y) - 16;
}

void Engine::process_input()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
            case sf::Keyboard::W:
                network.move(3);
                break;
            case sf::Keyboard::A:
                network.move(1);
                break;
            case sf::Keyboard::S:
                network.move(4);
                break;
            case sf::Keyboard::D:
                network.move(2);
                break;
            case sf::Keyboard::Escape:
                window.close();
                break;
            default:
                break;
            }//end switch
            break;
        }
        case sf::Event::Closed:
        {
            window.close();
            break;
        }
        default:
        {
            break;
        }
        }//end switch
    }//end while
}

void Engine::scrool_camera()
{
    sf::Vector2i diff = center - sf::Vector2i(camera.getCenter());
    camera.move(std::max(-1, std::min(diff.x, 1)), std::max(-1, std::min(diff.y, 1)));
}

void Engine::draw_frame()
{
    scrool_camera();
    window.setView(camera);
    map.draw(window);
    window.display();
}

void Engine::process_network(const Poco::DynamicStruct& json)
{
    switch(var2int(json["code"]))
    {
    case 5://show damage
    {
        break;
    }
    case 10://other player movement
    {
        int id = json["player"];
        map.players[id].set_dir(json["dir"]);
        map.players[id].move(json["x"], json["y"]);
        break;
    }
    case 54://remove monster
    {
        int id = json["id"];
        map.monsters.erase(id);
        break;
    }
    case 55://update map data
    {
        updateMap(json["data"].extract<Poco::DynamicStruct>());
        break;
    }
    case 71://other player join
    {
        int id = json["id"];
        map.players[id].setTexture(resourceManager.getTexture(json["looktype"], PLAYER));
        map.players[id].set_position(json["x"] + 1, json["y"] + 1);//server bug
        break;
    }
    case 100:
    case char2int("teleport"):
    {
        loadData(json["data"].extract<Poco::DynamicStruct>());
        break;
    }
    case 101://my movement
    {
        int x = json["x"];
        int y = json["y"];
        map.player.set_dir(json["dir"]);
        map.player.set_position(x, y);
        cameraCenterSmooth(x, y);
        break;
    }
    case 102://my back movement
    {
        break;
    }
    case 877://remove map item
    {
        int id = json["id"];
        map.items.erase(id);
        break;
    }
    case 878://new map item
    {
        auto& item = json["item"];
        int id = item["id"];
        map.items[id].setTexture(resourceManager.getTexture(item["item_id"], ITEM));
        map.items[id].set_position(item["x"], item["y"]);
        break;
    }
    case 1051://other player left
    {
        int id = json["player"]["id"];
        map.players.erase(id);
        break;
    }
    case 1504://other player yell
    {
        break;
    }
    case char2int("load_game"):
    {
        process_network(network.receiveInit());
        break;
    }
    default:
    {
        std::cout << json["code"].toString() << " NOT IMPLEMENTED\n";
        break;
    }
    }//end switch
}

void Engine::updateMap(const Poco::DynamicStruct& data)
{
    for(auto &i: data)
    {
        switch(str2int(i.first))
        {
        case char2int("moves"):
        {
            auto& array = i.second;
            for(sf::Uint8 i = 0; i < array.size(); ++i)
            {
                auto& v = array[i];
                int id = v["monster"];
                map.monsters[id].set_dir(v["dir"]);
                map.monsters[id].move(v["x"], v["y"]);
            }
            break;
        }
        case char2int("npc_moves"):
        {
            auto& array = i.second;
            for(sf::Uint8 i = 0; i < array.size(); ++i)
            {
                auto& v = array[i];
                int id = v["npc"];
                map.npcs[id].set_dir(v["dir"]);
                map.npcs[id].move(v["x"], v["y"]);
            }
            break;
        }
        case char2int("respawns"):
        {
            auto& array = i.second;
            for(sf::Uint8 i = 0; i < array.size(); ++i)
            {
                auto& v = array[i];
                int id = v["id"];
                map.monsters[id].setTexture(resourceManager.getTexture(v["looktype"], MONSTER), v["width"], v["height"]);
                map.monsters[id].set_position(v["x"], v["y"]);
            }
            break;
        }
        case char2int("yells"):
        {
            break;
        }
        default:
        {
            std::cout << i.first << " NOT IMPLEMENTED\n";
            break;
        }
        }//end switch
    }//end for
}

void Engine::loadData(const Poco::DynamicStruct& data)
{
    resourceManager.loadParallel(data);
    map.clear();

    auto& map_positions = data["map_positions"];
    int x = map_positions["PLAYER_X"];
    int y = map_positions["PLAYER_Y"];
    map.player.set_position(x, y);
    cameraCenterInstant(x, y);

    if(data.contains("map_data"))
    {
        auto& map_data = data["map_data"];
        for(sf::Uint8 i = 0; i < map_data.size(); ++i)
        {
            auto& map_tile = map_data[i];
            map.setTexture(resourceManager.getTexture(map_tile["source"], MAP_TILE), map_tile["x"], map_tile["y"]);
        }
    }
    else
    {
        map.setTexture(resourceManager.getTexture(data["map"]["id"], MAP_SINGLE), 0, 0);
    }

    auto& tiles = data["tiles"];
    for(sf::Uint8 i = 0; i < tiles.size(); ++i)
    {
        auto& tile = tiles[i];
        if(tile["type"] == 2)
        {
            map.doors.emplace_back();
            map.doors.back().setTexture(resourceManager.getTexture(tile["bg"]));
            map.doors.back().set_position(tile["x"], tile["y"]);
        }
    }

    auto& chests = data["chests"];
    for(sf::Uint8 i = 0; i < chests.size(); ++i)
    {
        auto& chest = chests[i];
        int id = chest["id"];
        if(chest["open"])
            map.chests[id].setTexture(resourceManager.getTexture(chest["type"], CHEST_OPENED));
        else
            map.chests[id].setTexture(resourceManager.getTexture(chest["type"], CHEST_CLOSED));
        map.chests[id].set_position(chest["x"], chest["y"]);
    }

    if(data.contains("map_items"))
    {
        auto& map_items = data["map_items"];
        for(sf::Uint8 i = 0; i < map_items.size(); ++i)
        {
            auto& item = map_items[i];
            int id = item["id"];
            map.items[id].setTexture(resourceManager.getTexture(item["item_id"], ITEM));
            map.items[id].set_position(item["x"], item["y"]);
        }
    }

    auto& monsters = data["monsters"];
    for(sf::Uint8 i = 0; i < monsters.size(); ++i)
    {
        auto& mns = monsters[i];
        int id = mns["id"];
        map.monsters[id].setTexture(resourceManager.getTexture(mns["looktype"], MONSTER), mns["width"], mns["height"]);
        map.monsters[id].set_position(mns["x"], mns["y"]);
    }

    auto& npcs = data["npcs"];
    for(sf::Uint8 i = 0; i < npcs.size(); ++i)
    {
        auto& npc = npcs[i];
        int id = npc["id"];
        map.npcs[id].setTexture(resourceManager.getTexture(npc["looktype"], NPC));
        map.npcs[id].set_position(npc["x"], npc["y"]);
    }

    auto& players = data["players"];
    for(sf::Uint8 i = 0; i < players.size(); ++i)
    {
        auto& player = players[i];
        int id = player["id"];
        map.players[id].setTexture(resourceManager.getTexture(player["looktype"], PLAYER));
        map.players[id].set_position(player["x"] + 1, player["y"] + 1);//server bug
    }
}
