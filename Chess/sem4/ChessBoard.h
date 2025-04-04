#pragma once
#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Piece.h"

class ChessBoard {
public:
    // Usuni�cie domy�lnego konstruktora kopiuj�cego
    ChessBoard(const ChessBoard&) = delete;
    ChessBoard& operator=(const ChessBoard&) = delete;

private:
    // Tablica wska�nik�w na figury
    std::unique_ptr<Piece> board[8][8];

    // Dodatkowe stany gry
    bool whiteKingMoved;   // Czy bia�y kr�l si� ruszy� (roszada)
    bool blackKingMoved;   // Czy czarny kr�l si� ruszy� (roszada)
    bool whiteRook1Moved;  // Czy bia�a wie�a (lewa) si� ruszy�a
    bool whiteRook2Moved;  // Czy bia�a wie�a (prawa) si� ruszy�a
    bool blackRook1Moved;  // Czy czarna wie�a (lewa) si� ruszy�a
    bool blackRook2Moved;  // Czy czarna wie�a (prawa) si� ruszy�a
    int enPassantCol;      // Kolumna dla bicia w przelocie (-1 je�li niedost�pne)

    // Metody pomocnicze
    bool isValidPosition(int row, int col) const;
    bool isPieceOfCurrentPlayer(int row, int col, bool isWhiteTurn) const;

    // Sprawdzenia zwi�zane z szachem
    bool isKingInCheck(PieceColor color) const;
    bool wouldMoveExposeKing(int fromRow, int fromCol, int toRow, int toCol, bool isWhiteTurn) const;



public:
    ChessBoard();

    // Inicjalizacja planszy
    void resetBoard();

    // Dost�p do stanu planszy
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