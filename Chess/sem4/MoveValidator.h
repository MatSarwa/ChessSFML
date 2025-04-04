//#pragma once
//#include "Board.h"
//#include <vector>
//#include <SFML/Graphics.hpp>
//
//// Klasa odpowiedzialna za walidacjê ruchów w grze szachowej
//class MoveValidator {
//private:
//    // Referencja do planszy
//    const Board& board;
//
//    // Pozycje królów
//    int whiteKingRow;
//    int whiteKingCol;
//    int blackKingRow;
//    int blackKingCol;
//
//    // Sprawdza czy po ruchu król bêdzie szachowany
//    bool isKingInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol, bool isWhite) const;
//
//    // Pomocnicze metody do sprawdzania ruchów
//    bool isPieceOwner(int row, int col, bool isWhite) const;
//    bool isSquareEmpty(int row, int col) const;
//    bool isOpponentPiece(int row, int col, bool isWhite) const;
//
//public:
//    MoveValidator(const Board& board);
//
//    // Aktualizuj pozycje królów
//    void updateKingsPositions();
//
//    // SprawdŸ czy ruch jest legalny
//    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, bool isWhite) const;
//
//    // Pobierz wszystkie mo¿liwe ruchy dla figury
//    std::vector<sf::Vector2i> getPossibleMoves(int row, int col, bool isWhite) const;
//
//    // SprawdŸ czy król jest w szachu
//    bool isKingInCheck(bool isWhite) const;
//
//    // SprawdŸ czy jest szach-mat
//    bool isCheckmate(bool isWhite) const;
//
//    // SprawdŸ czy jest pat
//    bool isStalemate(bool isWhite) const;
//
//    // Pobierz figury atakuj¹ce króla
//    std::vector<sf::Vector2i> getAttackingPieces(bool isWhite) const;
//
//    // Sprawdza, czy król mo¿e siê ruszyæ, aby wyjœæ spod szacha
//    bool canKingEscapeCheck(bool isWhite) const;
//
//    // Sprawdza, czy mo¿na zablokowaæ szachuj¹c¹ figurê
//    bool canBlockCheck(bool isWhite) const;
//
//    // Sprawdza, czy mo¿na zbiæ szachuj¹c¹ figurê
//    bool canCaptureAttacker(bool isWhite) const;
//};