//#pragma once
//#include "Board.h"
//#include <vector>
//#include <SFML/Graphics.hpp>
//
//// Klasa odpowiedzialna za walidacj� ruch�w w grze szachowej
//class MoveValidator {
//private:
//    // Referencja do planszy
//    const Board& board;
//
//    // Pozycje kr�l�w
//    int whiteKingRow;
//    int whiteKingCol;
//    int blackKingRow;
//    int blackKingCol;
//
//    // Sprawdza czy po ruchu kr�l b�dzie szachowany
//    bool isKingInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol, bool isWhite) const;
//
//    // Pomocnicze metody do sprawdzania ruch�w
//    bool isPieceOwner(int row, int col, bool isWhite) const;
//    bool isSquareEmpty(int row, int col) const;
//    bool isOpponentPiece(int row, int col, bool isWhite) const;
//
//public:
//    MoveValidator(const Board& board);
//
//    // Aktualizuj pozycje kr�l�w
//    void updateKingsPositions();
//
//    // Sprawd� czy ruch jest legalny
//    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, bool isWhite) const;
//
//    // Pobierz wszystkie mo�liwe ruchy dla figury
//    std::vector<sf::Vector2i> getPossibleMoves(int row, int col, bool isWhite) const;
//
//    // Sprawd� czy kr�l jest w szachu
//    bool isKingInCheck(bool isWhite) const;
//
//    // Sprawd� czy jest szach-mat
//    bool isCheckmate(bool isWhite) const;
//
//    // Sprawd� czy jest pat
//    bool isStalemate(bool isWhite) const;
//
//    // Pobierz figury atakuj�ce kr�la
//    std::vector<sf::Vector2i> getAttackingPieces(bool isWhite) const;
//
//    // Sprawdza, czy kr�l mo�e si� ruszy�, aby wyj�� spod szacha
//    bool canKingEscapeCheck(bool isWhite) const;
//
//    // Sprawdza, czy mo�na zablokowa� szachuj�c� figur�
//    bool canBlockCheck(bool isWhite) const;
//
//    // Sprawdza, czy mo�na zbi� szachuj�c� figur�
//    bool canCaptureAttacker(bool isWhite) const;
//};