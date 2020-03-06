#pragma once
#include <SFML/Window.hpp>
#include <memory>
#include <string>
#include <optional>

class MainGame
{
public:
    void init();
    void display();
private:
    std::unique_ptr<sf::Window> m_window;
};