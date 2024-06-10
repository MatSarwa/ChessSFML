#pragma once
#include "Piece.h"

class Bishop : public Piece {
public:
    bool canMove(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Goniec porusza siê po skosie
        if (std::abs(deltaX) == std::abs(deltaY)) {
            // Sprawdzenie czy na drodze biskupa nie ma ¿adnych pionków
            int stepX = (deltaX > 0) ? 1 : -1;
            int stepY = (deltaY > 0) ? 1 : -1;

            int checkX = fromCol + stepX;
            int checkY = fromRow + stepY;

            while (checkX != toCol && checkY != toRow) {
                if (board[checkY][checkX] != 0) {
                    return false; // Na drodze biskupa jest jakiœ pionek
                }
                checkX += stepX;
                checkY += stepY;
            }
            return true;
        }
        return false;
    }

    bool canCapture(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Goniec mo¿e zbijaæ pionek przeciwnika na skosie
        if (std::abs(deltaX) == std::abs(deltaY)) {
            int stepX = (deltaX > 0) ? 1 : -1;
            int stepY = (deltaY > 0) ? 1 : -1;

            int checkX = fromCol + stepX;
            int checkY = fromRow + stepY;

            while (checkX != toCol && checkY != toRow) {
                if (board[checkY][checkX] != 0) {
                    return false; // Na drodze biskupa jest jakiœ pionek
                }
                checkX += stepX;
                checkY += stepY;
            }
            return true;
        }
        return false;
    }

    bool canCaptureKing(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Goniec atakuje króla na polu docelowym
        if (std::abs(deltaX) == std::abs(deltaY)) {
            int stepX = (deltaX > 0) ? 1 : -1;
            int stepY = (deltaY > 0) ? 1 : -1;

            int checkX = fromCol + stepX;
            int checkY = fromRow + stepY;

            while (checkX != toCol && checkY != toRow) {
                if (board[checkY][checkX] != 0) {
                    return false; // Na drodze biskupa jest jakiœ pionek
                }
                checkX += stepX;
                checkY += stepY;
            }
            if (board[toRow][toCol] == -4 || board[toRow][toCol] == 4) {
                return true;
            }
        }
        return false;
    }


    bool canCapturePiece(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], int targetPieceValue, bool isWhite) const override{
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Goniec atakuje pionka na polu docelowym
        if (std::abs(deltaX) == std::abs(deltaY)) {
            int stepX = (deltaX > 0) ? 1 : -1;
            int stepY = (deltaY > 0) ? 1 : -1;

            int checkX = fromCol + stepX;
            int checkY = fromRow + stepY;

            while (checkX != toCol && checkY != toRow) {
                if (board[checkY][checkX] != 0) {
                    return false; // Na drodze biskupa jest jakiœ pionek
                }
                checkX += stepX;
                checkY += stepY;
            }
            if (board[toRow][toCol] == targetPieceValue) {
                return true;
            }
        }
        return false;
    }



};
