//#pragma once
//#include "Board.h"
//#include "MoveValidator.h"
//#include "ChessNotation.h"
//#include "GameState.h"
//#include "MoveHistory.h"
//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <string>
//#include <memory>
//
//// Enumy dla stanu gry
//enum class GameResult {
//    ONGOING,
//    WHITE_WIN,
//    BLACK_WIN,
//    DRAW
//};
//
//// G³ówna klasa kontrolera gry szachowej
//class Game {
//private:
//    // Stan gry
//    Board board;                         // Plansza szachowa
//    std::unique_ptr<MoveValidator> validator; // Walidator ruchów
//    std::unique_ptr<ChessNotation> notation;  // Konwerter notacji
//    std::unique_ptr<GameState> gameState;     // Stan gry
//    std::unique_ptr<MoveHistory> moveHistory; // Historia ruchów
//
//    // Bie¿¹ce zmienne gry
//    bool whiteTurn;                      // Czy teraz ruch bia³ych
//    int selectedPieceRow;                // Wiersz wybranej figury
//    int selectedPieceCol;                // Kolumna wybranej figury
//    bool pieceSelected;                  // Czy figura jest wybrana (zmieniona nazwa z isPieceSelected)
//
//    // Zmienne dla ostatniego ruchu
//    int lastMoveFromRow;
//    int lastMoveFromCol;
//    int lastMoveToRow;
//    int lastMoveToCol;
//    int lastCapturedPiece;
//    bool lastMoveWasCapture;
//    std::string lastMoveNotation;
//
//    // Pomocnicze bufory
//    std::vector<sf::Vector2i> possibleMoves;  // Mo¿liwe ruchy dla wybranej figury
//    std::vector<sf::Vector2i> attackingPieces; // Figury atakuj¹ce króla
//
//public:
//    // Konstruktor i destruktor
//    Game();
//    ~Game();
//
//    // Inicjalizacja i reset
//    void init();
//    void reset();
//
//    // Metody g³ównego cyklu gry
//    void update();
//    void handleInput(const sf::Event& event, const sf::Vector2i& mousePos);
//
//    // Metody zarz¹dzania ruchami
//    bool makeMove(int fromRow, int fromCol, int toRow, int toCol);
//    bool undoLastMove();
//    std::vector<sf::Vector2i> getPossibleMoves(int row, int col);
//
//    // Metody dotycz¹ce stanu gry
//    bool isWhiteTurn() const { return whiteTurn; }
//    bool isGameOver() const;
//    bool isCheck(bool forWhite) const;
//    bool isCheckmate(bool forWhite) const;
//    bool isStalemate(bool forWhite) const;
//    GameResult getGameResult() const;
//    std::string getResultReason() const;
//
//    // Pomocnicze metody
//    bool isPieceSelected() const;  // Zmieniona na funkcjê, która korzysta z pola pieceSelected
//    sf::Vector2i getSelectedPiecePos() const { return sf::Vector2i(selectedPieceRow, selectedPieceCol); }
//    bool wasLastMoveCapture() const { return lastMoveWasCapture; }
//    int getLastCapturedPiece() const { return lastCapturedPiece; }
//    std::string getLastMoveNotation() const { return lastMoveNotation; }
//
//    // Gettery dla komponentów gry
//    const Board& getBoard() const { return board; }
//    const MoveHistory& getMoveHistory() const { return *moveHistory; }
//
//    // Metody dla promocji pionka
//    bool needsPromotion() const;
//    bool promotePawn(int row, int col, int newPiece);
////};