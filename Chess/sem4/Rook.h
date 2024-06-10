#pragma once
#include "Piece.h"

class Rook : public Piece {
public:
    bool canMove(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Wie¿a mo¿e poruszaæ siê w pionie i poziomie
        if (deltaX == 0 && deltaY != 0) {
            // Sprawdzenie czy na drodze wie¿y nie ma ¿adnych pionków
            int step = (deltaY > 0) ? 1 : -1;
            int checkY = fromRow + step;

            while (checkY != toRow) {
                if (board[checkY][fromCol] != 0) {
                    return false; // Na drodze wie¿y jest jakiœ pionek
                }
                checkY += step;
            }
            return true;
        }
        if (deltaX != 0 && deltaY == 0) {
            // Sprawdzenie czy na drodze wie¿y nie ma ¿adnych pionków
            int step = (deltaX > 0) ? 1 : -1;
            int checkX = fromCol + step;

            while (checkX != toCol) {
                if (board[fromRow][checkX] != 0) {
                    return false; // Na drodze wie¿y jest jakiœ pionek
                }
                checkX += step;
            }
            return true;
        }
        return false;
    }

    bool canCapture(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Wie¿a mo¿e zbijaæ pionek przeciwnika na poziomie lub pionie
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
        return false;
    }

    bool canCaptureKing(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = toCol - fromCol;
        int deltaY = toRow - fromRow;

        // Wie¿a atakuje króla na polu docelowym
        if (toRow == fromRow || toCol == fromCol) {
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
            if (board[toRow][toCol] == -4 || board[toRow][toCol] == 4) {
                return true;
            }
        }
        return false;
    }





bool canCapturePiece(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], int targetPieceValue, bool isWhite) const override{
    int deltaX = toCol - fromCol;
    int deltaY = toRow - fromRow;

    // Wie¿a atakuje pionka na polu docelowym
    if (toRow == fromRow || toCol == fromCol) {
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
        if (board[toRow][toCol] == targetPieceValue) {
            return true;
        }
    }
    return false;
}

};