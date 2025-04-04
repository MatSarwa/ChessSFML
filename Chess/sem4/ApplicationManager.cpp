// ApplicationManager.cpp
#include "ApplicationManager.h"
#include "MenuScreen.h"
#include "OptionsScreen.h"
#include "GameScreen.h"
#include <iostream>

void ApplicationManager::initialize() {
    // Utworzenie okna
    window.create(sf::VideoMode(600, 600), "Chess Game", sf::Style::Close | sf::Style::Titlebar);

    // Inicjalizacja muzyki
    if (backgroundMusic.openFromFile("resources/audio/ noblade.mp3")) {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(musicVolume);
        if (isMusicEnabled) {
            backgroundMusic.play();
            std::cout << "Muzyka zosta�a uruchomiona pomy�lnie!" << std::endl;
        }

        else {
            std::cout << "B��D: Nie uda�o si� za�adowa� pliku muzyki 'resources/audio/blade.mp3'" << std::endl;
        }
    }

    // Utworzenie i dodanie ekran�w
    screens["menu"] = new MenuScreen(window);
    screens["options"] = new OptionsScreen(window, this); // Przeka� wska�nik this
    screens["game"] = new GameScreen(window);



    // Pobierz czasy z OptionsScreen
    optionsScreenPtr = new OptionsScreen(window, this);

    // Ustawienie pocz�tkowego ekranu
    setScreen("menu");
}

void ApplicationManager::setScreen(const std::string& screenName) {
    auto it = screens.find(screenName);
    if (it != screens.end()) {
        // Wywo�ywanie metody onExit dla aktualnego ekranu
        if (currentScreen) {
            currentScreen->onExit();
        }
        
        // Zmie� rozmiar okna je�li prze��czamy na ekran gry
        if (screenName == "game") {
            window.setSize(sf::Vector2u(1280, 720)); // Wi�kszy rozmiar dla ekranu gry
            // Aktualizuj widok
            sf::FloatRect visibleArea(0, 0, 1280, 720);
            window.setView(sf::View(visibleArea));

            GameScreen* gameScreen = static_cast<GameScreen*>(it->second);
            gameScreen->setPlayerTimes(whitePlayerTimeSeconds, blackPlayerTimeSeconds);

        }
        else if (currentScreen == screens["game"]) {
            // Je�li wychodzimy z ekranu gry, wr�� do standardowego rozmiaru
            window.setSize(sf::Vector2u(600, 600));
            sf::FloatRect visibleArea(0, 0, 600, 600);
            window.setView(sf::View(visibleArea));
        }
        
        // Ustaw nowy ekran i wywo�aj jego metod� onEnter
        currentScreen = it->second;
        currentScreen->onEnter();
    }
}

void ApplicationManager::toggleMusic(bool enable) {
    isMusicEnabled = enable;
    if (isMusicEnabled) {
        backgroundMusic.play();
    }
    else {
        backgroundMusic.pause();
    }
}

void ApplicationManager::setMusicVolume(float volume) {
    musicVolume = volume;
    backgroundMusic.setVolume(musicVolume);
}

bool ApplicationManager::getMusicEnabled() const {
    return isMusicEnabled;
}

void ApplicationManager::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (currentScreen == screens["game"]) {

                if (event.type == sf::Event::Resized) {
                    // Zapobiegnij zmniejszeniu okna poni�ej 600x600
                    sf::Vector2u currentSize = window.getSize();
                    if (currentSize.x < 1280 || currentSize.y < 720) {
                        sf::Vector2u newSize(
                            std::max(currentSize.x, 1280u),
                            std::max(currentSize.y, 720u)
                        );
                        window.setSize(newSize);
                    }
                    // Aktualizuj widok, aby dopasowa� go do nowego rozmiaru okna
                    sf::FloatRect visibleArea(0, 0, currentSize.x, currentSize.y);
                    window.setView(sf::View(visibleArea));
                }
            }

            else {
            // Obs�uga zmiany rozmiaru okna
                if (event.type == sf::Event::Resized) {
                    // Zapobiegnij zmniejszeniu okna poni�ej 600x600
                    sf::Vector2u currentSize = window.getSize();
                    if (currentSize.x < 600 || currentSize.y < 600) {
                        sf::Vector2u newSize(
                            std::max(currentSize.x, 600u),
                            std::max(currentSize.y, 600u)
                        );
                        window.setSize(newSize);
                    }
                    // Aktualizuj widok, aby dopasowa� go do nowego rozmiaru okna
                    sf::FloatRect visibleArea(0, 0, currentSize.x, currentSize.y);
                    window.setView(sf::View(visibleArea));
                }
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

void ApplicationManager::setPlayerTimes(int whiteTimeSeconds, int blackTimeSeconds) {
    whitePlayerTimeSeconds = whiteTimeSeconds;
    blackPlayerTimeSeconds = blackTimeSeconds;
}