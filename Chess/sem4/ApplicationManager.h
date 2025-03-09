#pragma once
#include <map>
#include <string>
#include "Screen.h"
#include "Button.h"
#include "MenuScreen.h"
#include "OptionsScreen.h"
#include "GameScreen.h"
class ApplicationManager {
private:
    sf::RenderWindow window;
    std::map<std::string, Screen*> screens;
    Screen* currentScreen;
public:
    ApplicationManager() : currentScreen(nullptr) {}
    ~ApplicationManager() {
        // Zwolnienie zasobów
        for (auto& pair : screens) {
            delete pair.second;
        }
    }
    void initialize() {
        // Utworzenie okna
        window.create(sf::VideoMode(600, 600), "Chess Game");
        
        // Utworzenie i dodanie ekranów
        screens["menu"] = new MenuScreen(window);
        screens["options"] = new OptionsScreen(window);
        screens["game"] = new GameScreen(window);
        // Ustawienie pocz¹tkowego ekranu
        setScreen("menu");
    }
    void setScreen(const std::string& screenName) {
        auto it = screens.find(screenName);
        if (it != screens.end()) {
            currentScreen = it->second;
            currentScreen->onEnter();
        }
    }
    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                // Obs³uga zmiany rozmiaru okna
                if (event.type == sf::Event::Resized) {
                    // Zapobiegnij zmniejszeniu okna poni¿ej 600x600
                    sf::Vector2u currentSize = window.getSize();
                    if (currentSize.x < 600 || currentSize.y < 600) {
                        sf::Vector2u newSize(
                            std::max(currentSize.x, 600u),
                            std::max(currentSize.y, 600u)
                        );
                        window.setSize(newSize);
                    }
                    // Aktualizuj widok, aby dopasowaæ go do nowego rozmiaru okna
                    sf::FloatRect visibleArea(0, 0, currentSize.x, currentSize.y);
                    window.setView(sf::View(visibleArea));
                }
                // Przekazanie wydarzenia do aktualnego ekranu
                if (currentScreen) {
                    std::string nextScreen = currentScreen->handleEvent(event);
                    if (!nextScreen.empty() && nextScreen != "current") {
                        setScreen(nextScreen);
                    }
                }
            }
            // Aktualizacja i renderowanie
            if (currentScreen) {
                currentScreen->update();
                window.clear();
                currentScreen->render();
                window.display();
            }
        }
    }
};