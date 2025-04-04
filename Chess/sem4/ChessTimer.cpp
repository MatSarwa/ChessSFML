#include "ChessTimer.h"
#include <sstream>
#include <iomanip>
#include <iostream>

ChessTimer::ChessTimer(sf::RenderWindow& win, const sf::Vector2f& pos, const sf::Vector2f& sz, bool isWhite)
    : window(&win),
    position(pos),
    size(sz),
    totalTimeSeconds(600.0f), // 10 minut
    remainingTimeSeconds(600.0f),
    isRunning(false),
    isWhitePlayer(isWhite),
    isLowOnTime(false)
{
    // Inicjalizacja czasu z wiêksz¹ precyzj¹
    lastUpdateTime = std::chrono::high_resolution_clock::now();

    // Inicjalizacja t³a
    background.setSize(size);
    background.setPosition(position);
    background.setFillColor(sf::Color(70, 70, 70));
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color(100, 100, 100));

    // Inicjalizacja tekstów
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        std::cerr << "Error loading font in ChessTimer!" << std::endl;
        // Próba za³adowania z alternatywnej œcie¿ki
        if (!font.loadFromFile("font.ttf")) {
            std::cerr << "Still couldn't load font from alternative path!" << std::endl;
        }
    }

    playerNameText.setFont(font);
    playerNameText.setString(isWhitePlayer ? "White" : "Black");
    playerNameText.setCharacterSize(18);
    playerNameText.setFillColor(sf::Color::White);
    playerNameText.setPosition(position.x + 10, position.y + 5);

    timeText.setFont(font);
    timeText.setString(formatTime());
    timeText.setCharacterSize(24);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(position.x + 10, position.y + 30);
}

void ChessTimer::render() {
    window->draw(background);
    window->draw(playerNameText);
    window->draw(timeText);
}

void ChessTimer::update() {
    if (isRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();

        // Oblicz czas w mikrosekundach dla wiêkszej dok³adnoœci
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastUpdateTime).count();

        // Konwersja na sekundy z zachowaniem wysokiej precyzji
        double elapsedSeconds = elapsed / 1000000.0;

        // Zabezpieczenie przed zbyt du¿ymi skokami czasu (np. po debugowaniu)
        if (elapsedSeconds > 0.1) {
            elapsedSeconds = 0.1; // Maksymalnie 100ms na raz, aby unikn¹æ du¿ych skoków
        }

        // Aktualizuj czas ostatniej aktualizacji
        lastUpdateTime = currentTime;

        // Odejmij up³yniêty czas
        remainingTimeSeconds -= static_cast<float>(elapsedSeconds);

        if (remainingTimeSeconds < 0) {
            remainingTimeSeconds = 0;
            isRunning = false;
        }
        isLowOnTime = remainingTimeSeconds < 60.0f;
        timeText.setFillColor(isLowOnTime ? sf::Color::Red : sf::Color::White);

        // Aktualizacja tekstu czasu
        timeText.setString(formatTime());
    }
}

void ChessTimer::start() {
    if (!isRunning) {
        isRunning = true;
        lastUpdateTime = std::chrono::high_resolution_clock::now();
    }
}

void ChessTimer::stop() {
    if (isRunning) {
        isRunning = false;
    }
}

void ChessTimer::reset(float initialTimeSeconds) {
    totalTimeSeconds = initialTimeSeconds;
    remainingTimeSeconds = initialTimeSeconds;
    isRunning = false;
    isLowOnTime = false;
    timeText.setFillColor(sf::Color::White);
    timeText.setString(formatTime());
    // Zresetuj te¿ czas ostatniej aktualizacji
    lastUpdateTime = std::chrono::high_resolution_clock::now();
}

void ChessTimer::addTime(float secondsToAdd) {
    remainingTimeSeconds += secondsToAdd;
    timeText.setString(formatTime());
}

std::string ChessTimer::formatTime() const {
    int minutes = static_cast<int>(remainingTimeSeconds) / 60;
    int seconds = static_cast<int>(remainingTimeSeconds) % 60;
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds;
    return ss.str();
}

void ChessTimer::setRemainingTime(int seconds) {
    // Konwersja z int do float, bo remainingTimeSeconds jest typu float
    remainingTimeSeconds = static_cast<float>(seconds);
    isLowOnTime = remainingTimeSeconds < 60.0f;

    // Aktualizuj kolor tekstu jeœli ma³o czasu
    timeText.setFillColor(isLowOnTime ? sf::Color::Red : sf::Color::White);

    // Aktualizuj wyœwietlany tekst
    timeText.setString(formatTime());

    // Zresetuj czas ostatniej aktualizacji
    lastUpdateTime = std::chrono::high_resolution_clock::now();

}