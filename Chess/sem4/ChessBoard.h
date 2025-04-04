#pragma once
#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Piece.h"

class ChessBoard {
public:
    // Usuniêcie domyœlnego konstruktora kopiuj¹cego
    ChessBoard(const ChessBoard&) = delete;
    ChessBoard& operator=(const ChessBoard&) = delete;

private:
    // Tablica wskaŸników na figury
    std::unique_ptr<Piece> board[8][8];

    // Dodatkowe stany gry
    bool whiteKingMoved;   // Czy bia³y król siê ruszy³ (roszada)
    bool blackKingMoved;   // Czy czarny król siê ruszy³ (roszada)
    bool whiteRook1Moved;  // Czy bia³a wie¿a (lewa) siê ruszy³a
    bool whiteRook2Moved;  // Czy bia³a wie¿a (prawa) siê ruszy³a
    bool blackRook1Moved;  // Czy czarna wie¿a (lewa) siê ruszy³a
    bool blackRook2Moved;  // Czy czarna wie¿a (prawa) siê ruszy³a
    int enPassantCol;      // Kolumna dla bicia w przelocie (-1 jeœli niedostêpne)

    // Metody pomocnicze
    bool isValidPosition(int row, int col) const;
    bool isPieceOfCurrentPlayer(int row, int col, bool isWhiteTurn) const;

    // Sprawdzenia zwi¹zane z szachem
    bool isKingInCheck(PieceColor color) const;
    bool wouldMoveExposeKing(int fromRow, int fromCol, int toRow, int toCol, bool isWhiteTurn) const;



public:
    ChessBoard();

    // Inicjalizacja planszy
    void resetBoard();

    // Dostêp do stanu planszy
    const Piece* getPieceAt(int row, int col) const;
    Piece* getPieceAt(int row, int col);
    void setPieceAt(int row, int col, std::unique_ptr<Piece> piece);

    // Ruchy
    std::vector<sf::Vector2i> getPossibleMoves(int row, int col) const;
    bool makeMove(int fromRow, int fromCol, int toRow, int toCol);

    // Promocja pionka
    void promotePawn(int row, int col, PieceType newType);

    // Sprawdzanie stanu gry
    bool isCheckmate(bool isWhiteTurn) const;
    bool isStalemate(bool isWhiteTurn) const;
    bool isInCheck(bool isWhitePlayer) const;

};