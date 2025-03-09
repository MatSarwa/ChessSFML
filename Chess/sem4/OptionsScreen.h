#pragma once
#include "Screen.h"
#include "Button.h"

class OptionsScreen : public Screen {
private:
    Button backButton;
    Button fullscreenButton;
    Button musicToggleButton;
    Button soundToggleButton;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Font titleFont;
    sf::Text titleText;

    bool isFullscreen;
    bool isMusicEnabled;
    bool isSoundEnabled;

public:
    OptionsScreen(sf::RenderWindow& win) :
        Screen(win),
        backButton(50, 500, 150, 40, "Powrót"),
        fullscreenButton(400, 180, 250, 40, "Pe³ny ekran: Wy³¹czony"),
        musicToggleButton(400, 250, 250, 40, "Muzyka: W³¹czona"),
        soundToggleButton(400, 320, 250, 40, "DŸwiêki: W³¹czone"),
        isFullscreen(false),
        isMusicEnabled(true),
        isSoundEnabled(true)
    {
        // £adowanie czcionki
        if (!titleFont.loadFromFile("resources/fonts/arial.ttf")) {
            // Próba za³adowania systemowej czcionki w przypadku b³êdu
            titleFont.loadFromFile("C:/Windows/Fonts/arial.ttf");
        }

        // Konfiguracja tekstu tytu³owego
        titleText.setFont(titleFont);
        titleText.setString("Opcje");
        titleText.setCharacterSize(40);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(350, 80);

        // £adowanie tekstury t³a
        if (!backgroundTexture.loadFromFile("resources/menu_background.png")) {
            // W przypadku b³êdu u¿yj koloru t³a
            backgroundSprite.setColor(sf::Color(50, 50, 100));
        }
        else {
            backgroundSprite.setTexture(backgroundTexture);
        }

        // Ustawienie kolorów przycisków
        sf::Color buttonColor(100, 100, 180);
        sf::Color hoverColor(130, 130, 220);

        backButton.setColors(buttonColor, hoverColor);
        fullscreenButton.setColors(buttonColor, hoverColor);
        musicToggleButton.setColors(buttonColor, hoverColor);
        soundToggleButton.setColors(buttonColor, hoverColor);
    }

    void onEnter() override {
        // Kod wykonywany przy wejœciu na ekran opcji
    }

    void onExit() override {
        // Kod wykonywany przy wyjœciu z ekranu opcji
    }

    std::string handleEvent(const sf::Event& event) override {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            if (backButton.isClicked(mousePos)) {
                return "menu";
            }
            else if (fullscreenButton.isClicked(mousePos)) {
                toggleFullscreen();
            }
            else if (musicToggleButton.isClicked(mousePos)) {
                toggleMusic();
            }
            else if (soundToggleButton.isClicked(mousePos)) {
                toggleSound();
            }
        }

        return "current"; // Pozostañ na tym samym ekranie
    }

    void update() override {
        // Aktualizacja stanu przycisków na podstawie pozycji myszy
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        backButton.update(mousePos);
        fullscreenButton.update(mousePos);
        musicToggleButton.update(mousePos);
        soundToggleButton.update(mousePos);
    }

    void render() override {
        window.draw(backgroundSprite);
        window.draw(titleText);

        backButton.render(window);
        fullscreenButton.render(window);
        musicToggleButton.render(window);
        soundToggleButton.render(window);
    }

private:
    void toggleFullscreen() {
        isFullscreen = !isFullscreen;

        if (isFullscreen) {
            window.create(sf::VideoMode::getDesktopMode(), "Chess Game", sf::Style::Fullscreen);
            fullscreenButton = Button(400, 180, 250, 40, "Pe³ny ekran: W³¹czony");
        }
        else {
            window.create(sf::VideoMode(800, 600), "Chess Game", sf::Style::Default);
            fullscreenButton = Button(400, 180, 250, 40, "Pe³ny ekran: Wy³¹czony");
        }

        // Aktualizacja kolorów przycisku
        sf::Color buttonColor(100, 100, 180);
        sf::Color hoverColor(130, 130, 220);
        fullscreenButton.setColors(buttonColor, hoverColor);
    }

    void toggleMusic() {
        isMusicEnabled = !isMusicEnabled;

        if (isMusicEnabled) {
            musicToggleButton = Button(400, 250, 250, 40, "Muzyka: W³¹czona");
            // Tutaj kod do w³¹czenia muzyki
        }
        else {
            musicToggleButton = Button(400, 250, 250, 40, "Muzyka: Wy³¹czona");
            // Tutaj kod do wy³¹czenia muzyki
        }

        // Aktualizacja kolorów przycisku
        sf::Color buttonColor(100, 100, 180);
        sf::Color hoverColor(130, 130, 220);
        musicToggleButton.setColors(buttonColor, hoverColor);
    }

    void toggleSound() {
        isSoundEnabled = !isSoundEnabled;

        if (isSoundEnabled) {
            soundToggleButton = Button(400, 320, 250, 40, "DŸwiêki: W³¹czone");
            // Tutaj kod do w³¹czenia dŸwiêków
        }
        else {
            soundToggleButton = Button(400, 320, 250, 40, "DŸwiêki: Wy³¹czone");
            // Tutaj kod do wy³¹czenia dŸwiêków
        }

        // Aktualizacja kolorów przycisku
        sf::Color buttonColor(100, 100, 180);
        sf::Color hoverColor(130, 130, 220);
        soundToggleButton.setColors(buttonColor, hoverColor);
    }
};