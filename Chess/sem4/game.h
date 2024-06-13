#pragma once

#include "Graph.h"
#include "Board.h"
#include "Piece.h"


class Game {
private:
    Graph graph;
    ChessBoard board;
    Piece* selectedPiece = nullptr;
    bool whiteTurn = true;
    std::vector<std::tuple<int, int, int>> attackers;
    std::vector<std::pair<int, int>> possibleKingMoves; // Dodane
    std::vector<std::pair<int, int>> possibleBlock; // Dodane
    std::pair<int, int> findKing(int kingValue);

    int whiteKingRow = -1;
    int whiteKingCol = -1;
    int blackKingRow = -1;
    int blackKingCol = -1;

public:
    // Konstruktor
    Game() : possibleKingMoves(possibleKingMoves) {}; // Poprawione

    void movePiece(int fromRow, int fromCol, int toRow, int toCol, bool isWhite);
    void GameLoop();
    bool isWhiteTurn() const;
    void toggleTurn();
    bool isCheck(bool isWhiteTurn, std::vector<std::tuple<int, int, int>>& attackers);
    bool isCheckmate(bool isWhiteTurn);
    bool isCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol, bool isWhiteTurn);
    bool canKingMove(int kingRow, int kingCol, bool isWhiteTurn, std::vector<std::pair<int, int>>& possibleKingMoves);
    Piece* createPiece(int pieceValue);
    bool canCaptureAttacker(int toRow, int toCol, const int board[8][8], const std::vector<std::tuple<int, int, int>>& attackers, bool isWhite);
    bool canBlockCheck(int kingRow, int kingCol, int attackerRow, int attackerCol, int attackerValue, bool isWhite, const std::vector<std::tuple<int, int, int>>& attackers, std::vector<std::pair<int, int>>& possibleBlock);
    void checkPromotion();
    void findKings(const int(*board)[8]);

};

