#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>

class ChessTimer {
private:
    sf::RenderWindow* window;
    sf::Font font;

    // Pozycja i wymiary
    sf::Vector2f position;
    sf::Vector2f size;

    // Elementy graficzne
    sf::RectangleShape background;
    sf::Text playerNameText;
    sf::Text timeText;

    // Czas
    float totalTimeSeconds;       // Ca³kowity przydzielony czas (w sekundach)
    float remainingTimeSeconds;   // Pozosta³y czas (w sekundach)
    bool isRunning;               // Czy zegar jest aktywny

    // Timestampy
    std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;

    // Flagi
    bool isWhitePlayer;           // Czy to zegar dla bia³ego gracza
    bool isLowOnTime;             // Flaga oznaczaj¹ca ma³o czasu (podœwietlenie na czerwono)

public:
    ChessTimer(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& size, bool isWhitePlayer);

    void render();
    void update();

    // Kontrola czasu
    void start();
    void stop();
    void reset(float initialTimeSeconds);
    void addTime(float secondsToAdd);  // Dodaje czas (np. dla bonusu za ruch)

    // Gettery
    float getRemainingTime() const { return remainingTimeSeconds; }
    bool isTimeExpired() const { return remainingTimeSeconds <= 0; }
    bool isActive() const { return isRunning; }

    void setRemainingTime(int seconds);

    // Formatowanie czasu
    std::string formatTime() const;
};