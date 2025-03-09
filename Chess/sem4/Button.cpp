#include "Button.h"

Button::Button(float x, float y, float width, float height, const std::string& buttonText, unsigned int fontSize)
    : isHovered(false),
    idleColor(sf::Color(100, 100, 100)),
    hoverColor(sf::Color(150, 150, 150)) {

    // Konfiguracja kszta�tu przycisku
    shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(idleColor);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);

    // Pr�ba za�adowania domy�lnej czcionki
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        // Je�li nie uda�o si� za�adowa� czcionki, spr�buj wbudowanej w system
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            // Mo�esz doda� wi�cej �cie�ek do sprawdzenia
            // Lub obs�u�y� b��d w inny spos�b
        }
    }

    // Konfiguracja tekstu przycisku
    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(fontSize);
    text.setFillColor(sf::Color::White);

    // Wy�rodkowanie tekstu na przycisku
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        x + (width - textBounds.width) / 2.0f,
        y + (height - textBounds.height) / 2.0f - textBounds.top
    );
}

void Button::setFont(const sf::Font& newFont) {
    font = newFont;
    text.setFont(font);

    // Ponowne wy�rodkowanie tekstu
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        shape.getPosition().x + (shape.getSize().x - textBounds.width) / 2.0f,
        shape.getPosition().y + (shape.getSize().y - textBounds.height) / 2.0f - textBounds.top
    );
}

void Button::setPosition(float x, float y) {
    shape.setPosition(x, y);

    // Aktualizacja pozycji tekstu
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        x + (shape.getSize().x - textBounds.width) / 2.0f,
        y + (shape.getSize().y - textBounds.height) / 2.0f - textBounds.top
    );
}

void Button::setColors(const sf::Color& idle, const sf::Color& hover) {
    idleColor = idle;
    hoverColor = hover;
    shape.setFillColor(idleColor);
}

bool Button::contains(const sf::Vector2f& point) const {
    return shape.getGlobalBounds().contains(point);
}

bool Button::isClicked(const sf::Vector2i& mousePos) const {
    return shape.getGlobalBounds().contains(sf::Vector2f(mousePos));
}

void Button::update(const sf::Vector2i& mousePos) {
    // Sprawdzenie czy mysz znajduje si� nad przyciskiem
    isHovered = contains(sf::Vector2f(mousePos));

    // Zmiana koloru w zale�no�ci od stanu
    if (isHovered) {
        shape.setFillColor(hoverColor);
    }
    else {
        shape.setFillColor(idleColor);
    }
}

void Button::render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}