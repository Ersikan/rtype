/*
** EPITECH PROJECT, 2021
** R-type
** File description:
** R-type
*/

#ifndef _APP_WINDOW_HPP_
#define _APP_WINDOW_HPP_

#include <functional>
#include <unordered_map>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include "sdk/interfaces/Iview.hpp"
#include "sdk/managers/event.hpp"

namespace window {
    constexpr auto WIDTH = 1800;
    constexpr auto HEIGHT = 980;
    constexpr auto FRAMERATE = 60;
}

class Window {
    std::string target_view;
    std::unique_ptr<Iview> view = nullptr;
    std::unordered_map<std::string, std::function<std::unique_ptr<Iview>(void)>> views;
    sf::RenderWindow sf_win;
    Event event;
//    Network network;
public:
    /**
     * Window Ctor
     * @param default_view is the first view used by the client
     */
    explicit Window(std::string default_view = "loading");
    ~Window();

    /**
     * main update loop
     */
    void update();
};

#endif