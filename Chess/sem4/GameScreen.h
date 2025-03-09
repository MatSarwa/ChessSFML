#pragma once
#include "Screen.h"
#include "Button.h"
#include "Game.h" 

class GameScreen : public Screen {
private:
    // Tymczasowo komentujemy klasê Game
    // Game game;
    Button backButton;
    sf::RectangleShape background;
    sf::CircleShape redCircle;

public:
    GameScreen(sf::RenderWindow& win) :
        Screen(win),
        backButton(50, 550, 100, 30, "Powrót")
    {
        // Tworzymy czarne t³o
        background.setSize(sf::Vector2f(win.getSize().x, win.getSize().y));
        background.setFillColor(sf::Color::Black);

        // Tworzymy czerwony okr¹g
        redCircle.setRadius(100);
        redCircle.setFillColor(sf::Color::Red);
        redCircle.setPosition(win.getSize().x / 2 - 100, win.getSize().y / 2 - 100);
    }

    void onEnter() override {
        // Tymczasowo puste
    }

    void onExit() override {
        // Tymczasowo puste
    }

    std::string handleEvent(const sf::Event& event) override {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (backButton.isClicked(mousePos)) {
                return "menu";
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                return "menu";
            }
        }

        return "current";
    }

    void update() override {
        // Aktualizacja stanu przycisku
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        backButton.update(mousePos);
    }

    void render() override {
        // Rysujemy czarne t³o
        window.draw(background);

        // Rysujemy czerwony okr¹g
        window.draw(redCircle);

        // Renderowanie przycisku powrotu
        backButton.render(window);
    }
};