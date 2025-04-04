#include "Slider.h"
#include <algorithm>

Slider::Slider(float x, float y, float width, float height, int min, int max) :
    minValue(min), maxValue(max), currentValue(max), isDragged(false)
{
    // T³o suwaka
    track.setSize(sf::Vector2f(width, height));
    track.setPosition(x, y);
    track.setFillColor(sf::Color(100, 100, 100));

    // Uchwyt suwaka
    handle.setSize(sf::Vector2f(20, height + 10));
    handle.setPosition(x + width - 10, y - 5);
    handle.setFillColor(sf::Color(200, 200, 200));
}

void Slider::render(sf::RenderWindow& window) {
    window.draw(track);
    window.draw(handle);
}

void Slider::update(sf::Vector2i mousePos) {
    // Logika podœwietlania, jeœli chcesz dodaæ efekt hover
}

bool Slider::isClicked(sf::Vector2i mousePos) {
    return handle.getGlobalBounds().contains(mousePos.x, mousePos.y);
}

int Slider::updateValue(sf::Vector2i mousePos) {
    if (!isDragged) return currentValue;
    float trackWidth = track.getSize().x;
    float trackX = track.getPosition().x;

    // Usuñ przesuniêcie -10.f i -20.f
    float newX = std::max(trackX, std::min(static_cast<float>(mousePos.x), trackX + trackWidth));
    handle.setPosition(newX, handle.getPosition().y);

    // Przelicz wartoœæ
    float percent = (newX - trackX) / trackWidth;
    currentValue = minValue + static_cast<int>(percent * (maxValue - minValue));
    return currentValue;
}

int Slider::getValue() const {
    return currentValue;
}

bool Slider::isDragging() const {
    return isDragged;
}

void Slider::stopDragging() {
    isDragged = false;
}

