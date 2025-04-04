//#pragma once
//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <string>
//
//class HistoryPanel {
//private:
//    sf::RenderWindow& window;          // Referencja do okna
//    sf::Vector2f position;             // Pozycja panelu
//    sf::Vector2f size;                 // Rozmiar panelu
//
//    sf::RectangleShape background;     // T³o panelu
//    sf::Font font;                     // Czcionka
//
//    std::vector<std::string> moves;    // Historia ruchów
//    int scrollOffset;                  // Przesuniêcie przewijania
//    int maxVisibleMoves;               // Maksymalna liczba widocznych ruchów
//
//    // Elementy UI
//    sf::RectangleShape scrollBar;      // Pasek przewijania
//    sf::RectangleShape scrollHandle;   // Uchwyt paska przewijania
//    bool isDraggingScroll;             // Czy uchwyt jest aktualnie przeci¹gany
//
//public:
//    HistoryPanel(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& size);
//
//    // Podstawowe metody
//    void render();
//    void handleEvent(const sf::Event& event);
//
//    // Metody zarz¹dzania histori¹
//    void addMove(const std::string& moveNotation);
//    void clear();
//
//    // Metody przewijania
//    void scrollUp();
//    void scrollDown();
//    void updateScrollbar();
//};