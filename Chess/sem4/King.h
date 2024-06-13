#pragma once
#include "Piece.h"


class King : public Piece {

public:

    bool canMove(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = std::abs(toCol - fromCol);
        int deltaY = std::abs(toRow - fromRow);

        // Król mo¿e poruszaæ siê o jedno pole w ka¿dym kierunku
        if (deltaX <= 1 && deltaY <= 1 && (deltaX != 0 || deltaY != 0)) {
            return true;
        }
        return false;
    }

    bool canCapture(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        // Najpierw sprawdzamy, czy król mo¿e siê przemieœciæ na docelowe pole
        if (!canMove(fromRow, fromCol, toRow, toCol, board, isWhite)) {
            return false;
        }

        // Sprawdzamy, czy na docelowym polu znajduje siê pionek przeciwnika
        int targetPiece = board[toRow][toCol];
        if (targetPiece != 0 && (isWhite ? targetPiece < 0 : targetPiece > 0)) {
            return true;
        }

        return false;
    }

    bool canCaptureKing(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        // Okreœlenie wartoœci wrogiego króla
        int enemyKingValue = isWhite ? -4 : 4;

        // Obliczenie ró¿nic odleg³oœci miêdzy from a to
        int deltaX = std::abs(toCol - fromCol);
        int deltaY = std::abs(toRow - fromRow);

        // Król mo¿e poruszaæ siê o jedno pole w ka¿dym kierunku
        if (deltaX <= 1 && deltaY <= 1 && (deltaX != 0 || deltaY != 0)) {
            // Sprawdzenie, czy na docelowej pozycji znajduje siê wrogi król
            if (board[toRow][toCol] == enemyKingValue) {
                return true; // Znaleziono wrogiego króla w odleg³oœci jednego pola
            }
        }

        return false; // Wrogi król nie znajduje siê w odleg³oœci jednego pola
    }




    bool canCapturePiece(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], int targetPieceValue, bool isWhite) const override {
        // Najpierw sprawdzamy, czy król mo¿e siê przemieœciæ na docelowe pole
        if (!canMove(fromRow, fromCol, toRow, toCol, board, isWhite)) {
           return false;

        }

        return false;
    }



};

