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

    std::string looktype = network.login();
    resourceManager.load_graphic(looktype, LOOKTYPE);
    map.player.set_texture(resourceManager.get_texture(looktype));
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

void Engine::cameraScrool()
{
    sf::Vector2i diff = center - sf::Vector2i(camera.getCenter());
    camera.move(std::max(-1, std::min(diff.x, 1)), std::max(-1, std::min(diff.y, 1)));
}

void Engine::cameraCenter(int x, int y)
{
    center.x = (32 * x) - 16;
    center.y = (32 * y) - 16;
    camera.setCenter(center.x, center.y);
}

void Engine::cameraSmooth(int x, int y)
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

void Engine::draw_frame()
{
    cameraScrool();
    window.setView(camera);
    map.draw(window);
    window.display();
}

void Engine::process_network(const Poco::DynamicStruct& json)
{
    auto code = json["code"];
    switch(var2int(code))
    {
    case 55:
    {
        auto data = json["data"].extract<Poco::DynamicStruct>();
        switch(str2int(data.begin()->first))
        {
        case char2int("moves"):
        {
            auto array = data.begin()->second;
            for(sf::Uint8 i = 0; i < array.size(); ++i)
            {
                auto v = array[i];
                map.monsters[v["monster"]].move(v["x"], v["y"]);
            }
            break;
        }
        case char2int("npc_moves"):
        {
            auto array = data.begin()->second;
            for(sf::Uint8 i = 0; i < array.size(); ++i)
            {
                auto v = array[i];
                map.NPCs[v["npc"]].move(v["x"], v["y"]);
            }
            break;
        }
        default:
        {
            std::cout << data.begin()->first << " NOT IMPLEMENTED\n";
            break;
        }
        }//end switch
        break;
    }
    case 100:
    case char2int("teleport"):
    {
        auto data = json["data"];
        auto map_positions = data["map_positions"];
        int x = map_positions["PLAYER_X"];
        int y = map_positions["PLAYER_Y"];
        cameraCenter(x, y);
        map.player.set_position(x, y);

        auto lmap = data["map"];
        switch(var2int(lmap["type"]))
        {
        case 1:
        {
            auto map_data = data["map_data"];
            for(sf::Uint8 i = 0; i < map_data.size(); ++i)
            {
                auto map_tile = map_data[i];
                const std::string& name = map_tile["source"];
                resourceManager.load_graphic(name, MAP_TILE);
                map.set_texture(resourceManager.get_texture(name), map_tile["x"], map_tile["y"]);
            }
            break;
        }
        case 2:
        {
            const std::string& name = lmap["id"];
            resourceManager.load_graphic(name, MAP_SINGLE);
            map.set_texture(resourceManager.get_texture(name), 0, 0);
            break;
        }
        default:
        {
            std::cout << lmap["type"].toString() << " NOT IMPLEMENTED\n";
            break;
        }
        }//end switch

        auto monsters = data["monsters"];
        for(sf::Uint8 i = 0; i < monsters.size(); ++i)
        {
            auto monster = monsters[i];
            int id = monster["id"];
            const std::string& looktype = monster["looktype"];
            resourceManager.load_graphic(looktype, MONSTER);
            map.monsters[id].set_texture(resourceManager.get_texture(looktype), monster["width"], monster["height"]);
            map.monsters[id].set_position(monster["x"], monster["y"]);
        }
        auto npcs = data["npcs"];
        for(sf::Uint8 i = 0; i < npcs.size(); ++i)
        {
            auto npc = npcs[i];
            int id = npc["id"];
            const std::string& looktype = npc["looktype"];
            resourceManager.load_graphic(looktype, NPC);
            map.NPCs[id].set_texture(resourceManager.get_texture(looktype));
            map.NPCs[id].set_position(npc["x"], npc["y"]);
        }
        break;
    }
    case 101:
    {
        int x = json["x"];
        int y = json["y"];
        cameraSmooth(x, y);
        map.player.set_position(x, y);
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
        std::cout << code.toString() << " NOT IMPLEMENTED\n";
        break;
    }
    }//end switch
}
