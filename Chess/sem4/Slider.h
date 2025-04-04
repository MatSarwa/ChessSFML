#pragma once
#include <SFML/Graphics.hpp>

class Slider {
private:
    sf::RectangleShape track;
    sf::RectangleShape handle;
    int minValue;
    int maxValue;
    int currentValue;
    bool isDragged;

public:
    Slider(float x, float y, float width, float height, int min, int max);

    void render(sf::RenderWindow& window);
    void update(sf::Vector2i mousePos);
    bool isClicked(sf::Vector2i mousePos);
    int updateValue(sf::Vector2i mousePos);
    int getValue() const;
    bool isDragging() const;
    void stopDragging();
    void startDragging() { isDragged = true; }

};