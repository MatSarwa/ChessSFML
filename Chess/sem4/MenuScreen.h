#pragma once
#include "Screen.h"
#include "Button.h"
class MenuScreen : public Screen {
private:
    Button playButton;
    Button optionsButton;
    Button exitButton;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
public:
    MenuScreen(sf::RenderWindow& win) :
        Screen(win),
        // Przyciski przesuni�te bardziej w lewo
        playButton(200, 200, 180, 40, "Play"),
        optionsButton(200, 260, 180, 40, "Options"),
        exitButton(200, 320, 180, 40, "Exit")
    {
        // Pr�ba za�adowania pliku
        if (!backgroundTexture.loadFromFile("resources/images/menu_background.png")) {
            // Je�li nie uda�o si� za�adowa�, utw�rz tekstur� zast�pcz�
            sf::Image fallbackImage;
            fallbackImage.create(600, 600, sf::Color(50, 50, 150));
            backgroundTexture.loadFromImage(fallbackImage);
        }
        backgroundSprite.setTexture(backgroundTexture);
        sf::Color buttonColor(100, 100, 180);
        sf::Color hoverColor(130, 130, 220);
        playButton.setColors(buttonColor, hoverColor);
        optionsButton.setColors(buttonColor, hoverColor);
        exitButton.setColors(buttonColor, hoverColor);

        // Pocz�tkowy rozmiar okna (ale bez ograniczenia minimum)
        window.setSize(sf::Vector2u(600, 600));
    }


    void render() override {
        // Ustaw widok, aby obejmowa� ca�e okno
        sf::Vector2u windowSize = window.getSize();
        sf::View view(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
        window.setView(view);
        // Dostosuj t�o do rozmiaru okna
        backgroundSprite.setPosition(0, 0);
        backgroundSprite.setScale(
            (float)windowSize.x / backgroundTexture.getSize().x,
            (float)windowSize.y / backgroundTexture.getSize().y
        );
        // Rysowanie
        window.draw(backgroundSprite);
        playButton.render(window);
        optionsButton.render(window);
        exitButton.render(window);
    }
    // Reszta kodu bez zmian
    void onEnter() override {
        // Kod wykonywany przy wej�ciu na ekran
    }
    void onExit() override {
        // Kod wykonywany przy wyj�ciu z ekranu
    }
    std::string handleEvent(const sf::Event& event) override {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (playButton.isClicked(mousePos)) {
                return "game";
            }
            else if (optionsButton.isClicked(mousePos)) {
                return "options";
            }
            else if (exitButton.isClicked(mousePos)) {
                window.close();
            }
        }
        return "current"; // Pozosta� na tym samym ekranie
    }


    void update() override {
        // Pobierz aktualn� pozycj� myszy
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Zaktualizuj stan przycisk�w
        playButton.update(mousePos);
        optionsButton.update(mousePos);
        exitButton.update(mousePos);
    }
};