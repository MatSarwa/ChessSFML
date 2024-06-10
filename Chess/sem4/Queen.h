#pragma once
#include "Piece.h"
#include <cmath>

class Queen : public Piece {
public:
    bool canMove(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Królowa porusza siê jak wie¿a lub goniec
        if ((deltaX == 0 && deltaY != 0) || (deltaX != 0 && deltaY == 0)) {
            return true;
        }
        if (std::abs(deltaX) == std::abs(deltaY)) {
            return true;
        }
        // Królowa mo¿e poruszaæ siê w ka¿dym kierunku o dowoln¹ liczbê pól
        return (deltaX == 0 || deltaY == 0 || std::abs(deltaX) == std::abs(deltaY));
    }

    bool canCapture(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        // Królowa mo¿e zbijaæ pionek przeciwnika na polu docelowym, tak jak wie¿a lub goniec
        if (toRow == fromRow || toCol == fromCol) {
            int deltaX = toCol - fromCol;
            int deltaY = toRow - fromRow;

            if (deltaX == 0 && deltaY != 0) {
                if (deltaY > 0) {
                    for (int i = fromRow + 1; i < toRow; ++i) {
                        if (board[i][fromCol] != 0) {
                            return false;
                        }
                    }
                }
                else {
                    for (int i = fromRow - 1; i > toRow; --i) {
                        if (board[i][fromCol] != 0) {
                            return false;
                        }
                    }
                }
                return true;
            }
            if (deltaX != 0 && deltaY == 0) {
                if (deltaX > 0) {
                    for (int i = fromCol + 1; i < toCol; ++i) {
                        if (board[fromRow][i] != 0) {
                            return false;
                        }
                    }
                }
                else {
                    for (int i = fromCol - 1; i > toCol; --i) {
                        if (board[fromRow][i] != 0) {
                            return false;
                        }
                    }
                }
                return true;
            }
        }
        else if (std::abs(toRow - fromRow) == std::abs(toCol - fromCol)) {
            int deltaX = (toCol - fromCol > 0) ? 1 : -1;
            int deltaY = (toRow - fromRow > 0) ? 1 : -1;

            int checkX = fromCol + deltaX;
            int checkY = fromRow + deltaY;

            while (checkX != toCol && checkY != toRow) {
                if (board[checkY][checkX] != 0) {
                    return false; // Na drodze królowej jest jakiœ pionek
                }
                checkX += deltaX;
                checkY += deltaY;
            }
            return true;
        }
        return false;
    }

    bool canCaptureKing(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Królowa atakuje króla na polu docelowym
        if ((deltaX == 0 && deltaY != 0) || (deltaX != 0 && deltaY == 0)) {
            int step;
            if (deltaX == 0) {
                step = (deltaY > 0) ? 1 : -1;
                for (int i = fromRow + step; i != toRow; i += step) {
                    if (board[i][fromCol] != 0) {
                        return false; // Na drodze królowej jest jakiœ pionek
                    }
                }
            }
            else if (deltaY == 0) {
                step = (deltaX > 0) ? 1 : -1;
                for (int i = fromCol + step; i != toCol; i += step) {
                    if (board[fromRow][i] != 0) {
                        return false; // Na drodze królowej jest jakiœ pionek
                    }
                }
            }
            return true;
        }
        if (std::abs(deltaX) == std::abs(deltaY)) {
            int stepX = (deltaX > 0) ? 1 : -1;
            int stepY = (deltaY > 0) ? 1 : -1;

            int checkX = fromCol + stepX;
            int checkY = fromRow + stepY;

            while (checkX != toCol && checkY != toRow) {
                if (board[checkY][checkX] != 0) {
                    return false; // Na drodze królowej jest jakiœ pionek
                }
                checkX += stepX;
                checkY += stepY;
            }
            return true;
        }
        return false;
    }



    bool canCapturePiece(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], int targetPieceValue, bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Biskup atakuje pionka na polu docelowym
        if (std::abs(deltaX) == std::abs(deltaY)) { // Biskup
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
        else if (toRow == fromRow || toCol == fromCol) { // Wie¿a
            int step;
            if (deltaX == 0) {
                step = (deltaY > 0) ? 1 : -1;
                for (int i = fromRow + step; i != toRow; i += step) {
                    if (board[i][fromCol] != 0) {
                        return false; // Na drodze wie¿y jest jakiœ pionek
                    }
                }
            }
            else if (deltaY == 0) {
                step = (deltaX > 0) ? 1 : -1;
                for (int i = fromCol + step; i != toCol; i += step) {
                    if (board[fromRow][i] != 0) {
                        return false; // Na drodze wie¿y jest jakiœ pionek
                    }
                }
            }
        }

        if (board[toRow][toCol] == targetPieceValue) {
            return true;
        }
        return false;
    }





};

