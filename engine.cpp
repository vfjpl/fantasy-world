#include "engine.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>

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

Engine::Engine()
{
    setup_window(true);
    network.login();
    network.sendInit(window.getSize());
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
        int x = json["x"];
        int y = json["y"];
        const std::string& looktype = json["looktype"];

        resourceManager.loadGraphic(looktype, PLAYER);
        map.players[id].set_texture(resourceManager.getTexture(looktype, PLAYER));
        map.players[id].set_position(x + 1, y + 1);//server bug
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
    case 102://back movement
    {
        break;
    }
    case 1051://other player left
    {
        auto& player = json["player"];
        int id = player["id"];
        map.players.erase(id);
        break;
    }
    case 1504://other player yell
    {
        break;
    }
    case char2int("load_game"):
    {
        map.clear();
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
                const std::string& looktype = v["looktype"];

                resourceManager.loadGraphic(looktype, MONSTER);
                map.monsters[id].set_texture(resourceManager.getTexture(looktype, MONSTER), v["width"], v["height"]);
                map.monsters[id].set_position(v["x"], v["y"]);
            }
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
    resourceManager.loadGraphic(network.getPlayerLooktype());
    map.player.set_texture(resourceManager.getTexture(network.getPlayerLooktype()));

    auto& map_positions = data["map_positions"];
    int x = map_positions["PLAYER_X"];
    int y = map_positions["PLAYER_Y"];
    map.player.set_position(x, y);
    cameraCenterInstant(x, y);

    auto& lmap = data["map"];
    switch(var2int(lmap["type"]))
    {
    case 1:
    {
        auto& map_data = data["map_data"];
        for(sf::Uint8 i = 0; i < map_data.size(); ++i)
        {
            auto& map_tile = map_data[i];
            const std::string& name = map_tile["source"];

            resourceManager.loadGraphic(name, MAP_TILE);
            map.set_texture(resourceManager.getTexture(name, MAP_TILE), map_tile["x"], map_tile["y"]);
        }
        break;
    }
    case 2:
    {
        const std::string& name = lmap["id"];

        resourceManager.loadGraphic(name, MAP_SINGLE);
        map.set_texture(resourceManager.getTexture(name, MAP_SINGLE), 0, 0);
        break;
    }
    default:
    {
        std::cout << lmap["type"].toString() << " NOT IMPLEMENTED\n";
        break;
    }
    }//end switch

    auto& monsters = data["monsters"];
    for(sf::Uint8 i = 0; i < monsters.size(); ++i)
    {
        auto& monster = monsters[i];
        int id = monster["id"];
        const std::string& looktype = monster["looktype"];

        resourceManager.loadGraphic(looktype, MONSTER);
        map.monsters[id].set_texture(resourceManager.getTexture(looktype, MONSTER), monster["width"], monster["height"]);
        map.monsters[id].set_position(monster["x"], monster["y"]);
    }
    auto& npcs = data["npcs"];
    for(sf::Uint8 i = 0; i < npcs.size(); ++i)
    {
        auto& npc = npcs[i];
        int id = npc["id"];
        const std::string& looktype = npc["looktype"];

        resourceManager.loadGraphic(looktype, NPC);
        map.npcs[id].set_texture(resourceManager.getTexture(looktype, NPC));
        map.npcs[id].set_position(npc["x"], npc["y"]);
    }
    auto& players = data["players"];
    for(sf::Uint8 i = 0; i < players.size(); ++i)
    {
        auto& player = players[i];
        int id = player["id"];
        int x = player["x"];
        int y = player["y"];
        const std::string& looktype = player["looktype"];

        resourceManager.loadGraphic(looktype, PLAYER);
        map.players[id].set_texture(resourceManager.getTexture(looktype, PLAYER));
        map.players[id].set_position(x + 1, y + 1);//server bug
    }
}
