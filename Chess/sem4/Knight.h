#pragma once
#include "Piece.h"

class Knight : public Piece {
public:
    bool canMove(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = std::abs(toCol - fromCol);
        int deltaY = std::abs(toRow - fromRow);

        // Skoczek porusza siê w kszta³cie litery "L"
        if ((deltaX == 2 && deltaY == 1) || (deltaX == 1 && deltaY == 2)) {
            return true;
        }
        return false;
    }

    bool canCapture(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        // Skoczek mo¿e zbiæ pionek przeciwnika na polu docelowym
        return true;
    }

    bool canCaptureKing(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = std::abs(toCol - fromCol);
        int deltaY = std::abs(toRow - fromRow);

        // Skoczek atakuje króla na polu docelowym
        if ((deltaX == 2 && deltaY == 1) || (deltaX == 1 && deltaY == 2)) {
            if (board[toRow][toCol] == -4 || board[toRow][toCol] == 4) {
                return true;
            }
        }
        return false;
    }

    bool canCapturePiece(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], int targetPieceValue, bool isWhite) const override {
        int deltaX = std::abs(toCol - fromCol);
        int deltaY = std::abs(toRow - fromRow);

        // Skoczek atakuje pionka na polu docelowym
        if ((deltaX == 1 && deltaY == 2) || (deltaX == 2 && deltaY == 1)) {
            if (board[toRow][toCol] == targetPieceValue) {
                return true;
            }
        }
        return false;
    }




};

