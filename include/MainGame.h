#pragma once
#include <SFML/Window.hpp>
#include <memory>
#include <string>
#include <optional>
#include "LangtonAnt.h"

class MainGame
{
public:
    void init(Configuration conf);
    void display();
private:
    std::unique_ptr<sf::Window> m_window;
    Configuration m_conf;
};