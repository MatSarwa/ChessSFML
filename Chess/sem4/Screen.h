#pragma once
#include <SFML/Graphics.hpp>

class Screen {
protected:
    sf::RenderWindow& window;

public:
    Screen(sf::RenderWindow& win) : window(win) {}
    virtual ~Screen() = default;

    virtual void onEnter() = 0;
    virtual void onExit() = 0;
    virtual std::string handleEvent(const sf::Event& event) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
};