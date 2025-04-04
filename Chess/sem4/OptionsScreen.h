#pragma once
#include "Screen.h"
#include "Button.h"
#include "Slider.h" 
#include "TimeInput.h" // Dodaj nowy komponent TimeInputField

class ApplicationManager;

class OptionsScreen : public Screen {
private:
    Button backButton;
    Button musicToggleButton;
    Slider volumeSlider;

    // Pola do wprowadzania czasu dla graczy (dodane)
    TimeInputField whiteTimeInput;
    TimeInputField blackTimeInput;
    Button sameTimeButton;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Font titleFont;
    sf::Text titleText;
    sf::Text volumeText;

    // Tytu³ ustawieñ timera (dodane)
    sf::Text timerSettingsTitle;

    bool isMusicEnabled;
    int volumeLevel; // Poziom g³oœnoœci od 0 do 100

    ApplicationManager* appManager;

public:
    OptionsScreen(sf::RenderWindow& win, ApplicationManager* manager = nullptr) :
        Screen(win),
        backButton(50, 500, 150, 40, "Back"),
        musicToggleButton(175, 100, 250, 40, "Music: On"),
        volumeSlider(175, 150, 250, 20, 0, 100),
        // Inicjalizacja pól czasu (dodane)
        whiteTimeInput(sf::Vector2f(175, 390), sf::Vector2f(200, 40), "White Player Time"),
        blackTimeInput(sf::Vector2f(175, 470), sf::Vector2f(200, 40), "Black Player Time"),
        sameTimeButton(440, 430, 80, 30, "Sync"),
        isMusicEnabled(false),
        volumeLevel(100),
        appManager(manager)
    {
        // £adowanie czcionki
        if (!titleFont.loadFromFile("resources/fonts/arial.ttf")) {
            titleFont.loadFromFile("C:/Windows/Fonts/arial.ttf");
        }

        // Konfiguracja tekstu tytu³owego
        titleText.setFont(titleFont);
        titleText.setString("Options");
        titleText.setCharacterSize(40);
        titleText.setFillColor(sf::Color::White);

        sf::FloatRect textRect = titleText.getLocalBounds();
        titleText.setOrigin(textRect.width / 2, 0);
        titleText.setPosition(300, 20);

        // Konfiguracja tekstu g³oœnoœci
        volumeText.setFont(titleFont);
        volumeText.setCharacterSize(20);
        volumeText.setFillColor(sf::Color::White);
        volumeText.setPosition(175, 200);
        updateVolumeText();

        // Konfiguracja tytu³u ustawieñ timera (dodane)
        timerSettingsTitle.setFont(titleFont);
        timerSettingsTitle.setString("Chess Timer Settings");
        timerSettingsTitle.setCharacterSize(24);
        timerSettingsTitle.setFillColor(sf::Color::White);
        timerSettingsTitle.setPosition(175, 270);

        // Ustawienie domyœlnych czasów (15 minut)
        whiteTimeInput.setTime(0, 5);
        blackTimeInput.setTime(0, 5);

        // £adowanie t³a
        if (!backgroundTexture.loadFromFile("resources/images/menu_background.png")) {
            sf::Image fallbackImage;
            fallbackImage.create(600, 600, sf::Color(50, 50, 150));
            backgroundTexture.loadFromImage(fallbackImage);
        }
        backgroundSprite.setTexture(backgroundTexture);

        // Ustawienie kolorów przycisków
        sf::Color buttonColor(100, 100, 180);
        sf::Color hoverColor(130, 130, 220);

        backButton.setColors(buttonColor, hoverColor);
        musicToggleButton.setColors(buttonColor, hoverColor);
        sameTimeButton.setColors(buttonColor, hoverColor);


    }

    void updateVolumeText() {
        volumeText.setString("Volume: " + std::to_string(volumeLevel) + "%");
    }

    std::string handleEvent(const sf::Event& event) override {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Obs³uga zdarzeñ dla pól czasu (dodane)
        whiteTimeInput.handleEvent(event, mousePos);
        blackTimeInput.handleEvent(event, mousePos);

        if (event.type == sf::Event::MouseButtonPressed) {
            if (backButton.isClicked(mousePos)) {
                return "menu";
            }
            else if (musicToggleButton.isClicked(mousePos)) {
                toggleMusic();
            }
            else if (volumeSlider.isClicked(mousePos)) {
                volumeSlider.startDragging();
                volumeLevel = volumeSlider.updateValue(mousePos);
                updateVolumeText();
                updateVolume();
            }
            else if (sameTimeButton.isClicked(mousePos)) {
                // Kopiuj czas z bia³ego do czarnego
                blackTimeInput.setTime(whiteTimeInput.getMinutes(), whiteTimeInput.getSeconds());
            }
        }
        else if (event.type == sf::Event::MouseMoved) {
            if (volumeSlider.isDragging()) {
                volumeLevel = volumeSlider.updateValue(mousePos);
                updateVolumeText();
                updateVolume();
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            volumeSlider.stopDragging();
        }

        return "current";
    }

    void update() override {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        backButton.update(mousePos);
        musicToggleButton.update(mousePos);
        volumeSlider.update(mousePos);
        sameTimeButton.update(mousePos);

        // Aktualizacja pól czasu (dodane)
        whiteTimeInput.update(mousePos);
        blackTimeInput.update(mousePos);
    }

    void render() override {
        // Ustaw widok
        sf::Vector2u windowSize = window.getSize();
        sf::View view(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
        window.setView(view);

        // Dostosuj t³o
        backgroundSprite.setPosition(0, 0);
        backgroundSprite.setScale(
            (float)windowSize.x / backgroundTexture.getSize().x,
            (float)windowSize.y / backgroundTexture.getSize().y
        );

        window.draw(backgroundSprite);
        window.draw(titleText);
        window.draw(volumeText);
        window.draw(timerSettingsTitle);

        backButton.render(window);
        musicToggleButton.render(window);
        volumeSlider.render(window);
        sameTimeButton.render(window);

        // Renderowanie pól czasu (dodane)
        whiteTimeInput.render(window);
        blackTimeInput.render(window);
    }

    void onEnter() override {
        // Kod wykonywany przy wejœciu na ekran
    }

    void onExit() override {
        
        updateTimers();


    }

private:
    void toggleMusic() {
        isMusicEnabled = !isMusicEnabled;

        if (isMusicEnabled) {
            musicToggleButton.setText("Music: On");
        }
        else {
            musicToggleButton.setText("Music: Off");
        }

        updateVolume();
    }

    void updateVolume() {
        // Wywo³aj funkcjê ApplicationManager tylko jeœli wskaŸnik jest prawid³owy
        if (appManager) {
            appManager->setMusicVolume(isMusicEnabled ? volumeLevel : 0);
        }
    }


    void updateTimers() {
        if (appManager) {
            appManager->setWhitePlayerTime(whiteTimeInput.getTotalSeconds());
            appManager->setBlackPlayerTime(blackTimeInput.getTotalSeconds());
        }
    }


  //  int getTime(TimeInputField playerColor) const { return playerColor.getTotalSeconds(); }




};