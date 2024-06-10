#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
    bool canMove(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Pionek mo¿e poruszyæ siê o jedno pole do przodu
        if (isWhite) {
            if ((deltaY == -1 || (fromRow == 6 && deltaY == -2)) && deltaX == 0) {
                return true;
            }
        }
        else {
            if ((deltaY == 1 || (fromRow == 1 && deltaY == 2)) && deltaX == 0) {
                return true;
            }
        }
        return false;
    }

    bool canCapture(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Pionek mo¿e zbijaæ pionek przeciwnika na skosie
        if (std::abs(deltaX) == 1 && ((isWhite && deltaY == -1) || (!isWhite && deltaY == 1))) {
            return true;
        }
        return false;
    }   

    bool canCaptureKing(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Pionek mo¿e zbijaæ króla na skosie
        if (std::abs(deltaX) == 1 && std::abs(deltaY) == 1) {
            if (isWhite && board[toRow][toCol] == -4) {
                return true;
            }
            else if (!isWhite && board[toRow][toCol] == 4) {
                return true;
            }
        }
        return false;
    }

    bool canCapturePiece(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], int targetPieceValue, bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Sprawdzenie czy na celu jest figura przeciwnika
        if ((isWhite && board[toRow][toCol] < 0) || (!isWhite && board[toRow][toCol] > 0)) {
            // Pionek mo¿e zbijaæ figury przeciwnika na skosie
            if (std::abs(deltaX) == 1 && ((isWhite && deltaY == -1) || (!isWhite && deltaY == 1))) {
                return true;
            }
        }

        return false;
    }





};
