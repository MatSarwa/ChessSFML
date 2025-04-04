#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Struktura reprezentuj¹ca ruch szachowy
struct ChessMove {
    std::string notation;
    bool isWhiteMove;
    bool isCheck;
    bool isCheckmate;

    ChessMove(const std::string& note, bool white, bool check = false, bool mate = false)
        : notation(note), isWhiteMove(white), isCheck(check), isCheckmate(mate) {
    }
};

class MoveHistoryPanel {
private:
    sf::RenderWindow* window;
    sf::RectangleShape background;
    sf::Font font;
    sf::Text titleText;
    sf::Vector2f position;
    sf::Vector2f size;
    std::vector<ChessMove> moves;

    // Scrollowanie
    sf::RectangleShape scrollbar;
    sf::RectangleShape scrollbarThumb;
    int scrollOffset;
    int maxVisibleMoves;
    bool isDraggingScrollbar;

public:
    MoveHistoryPanel(sf::RenderWindow& win, const sf::Vector2f& pos, const sf::Vector2f& sz);

    // Podstawowe operacje
    void render();
    void addMove(const ChessMove& move);
    void clear();
    void handleEvent(const sf::Event& event);

    // Nowe metody do obs³ugi historii ruchów
    const std::vector<ChessMove>& getMoves() const;
    const ChessMove& getLastMove() const;
    void removeLastMove();

private:
    void scroll(int delta);
};