#pragma once
#include <SFML/Graphics.hpp>
#include "ChessBoard.h"
#include <functional>
#include <vector>

class BoardView {
private:
    sf::RenderWindow* window;         // WskaŸnik do okna
    sf::Texture boardTexture;         // Tekstura planszy
    sf::Sprite boardSprite;           // Sprite planszy
    sf::Texture piecesTextures[12];   // Tekstury figur
    float pieceScales[12];            // Skala dla ka¿dej figury

    // Podœwietlenia
    sf::RectangleShape selectedPieceHighlight;
    sf::RectangleShape possibleMoveHighlight;
    std::vector<sf::Vector2i> possibleMoves;

    // Referencja do modelu planszy
    const ChessBoard& board;

    // Sta³e
    int squareSize;                  // Rozmiar pojedynczego pola
    sf::Vector2f boardPosition;      // Pozycja planszy na ekranie

public:
    BoardView(sf::RenderWindow& window, const ChessBoard& board);

    // Podstawowe metody
    void render();
    void update();

    // Konwersja wspó³rzêdnych
    sf::Vector2i getBoardPosition(const sf::Vector2i& screenPos) const;
    sf::Vector2f getScreenPosition(int row, int col) const;

    // Obs³uga podœwietleñ
    void highlightSelectedPiece(int row, int col);
    void clearHighlights();
    void setPossibleMoves(const std::vector<sf::Vector2i>& moves);

    // Animacje
    void animateMove(int fromRow, int fromCol, int toRow, int toCol);
    void showPromotionDialog(int row, int col, bool isWhite, std::function<void(int)> callback);

    // Pomocnicze
    float getBoardWidth() const;
    float getBoardHeight() const;
    sf::Vector2f getBoardCenter() const;
};