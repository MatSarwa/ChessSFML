#pragma once
#include "Piece.h"


class King : public Piece {

public:

    bool canMove(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        int deltaX = std::abs(toCol - fromCol);
        int deltaY = std::abs(toRow - fromRow);

        // Kr�l mo�e porusza� si� o jedno pole w ka�dym kierunku
        if (deltaX <= 1 && deltaY <= 1 && (deltaX != 0 || deltaY != 0)) {
            return true;
        }
        return false;
    }

    bool canCapture(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        // Najpierw sprawdzamy, czy kr�l mo�e si� przemie�ci� na docelowe pole
        if (!canMove(fromRow, fromCol, toRow, toCol, board, isWhite)) {
            return false;
        }

        // Sprawdzamy, czy na docelowym polu znajduje si� pionek przeciwnika
        int targetPiece = board[toRow][toCol];
        if (targetPiece != 0 && (isWhite ? targetPiece < 0 : targetPiece > 0)) {
            return true;
        }

        return false;
    }

    bool canCaptureKing(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const override {
        // Okre�lenie warto�ci wrogiego kr�la
        int enemyKingValue = isWhite ? -4 : 4;

        // Obliczenie r�nic odleg�o�ci mi�dzy from a to
        int deltaX = std::abs(toCol - fromCol);
        int deltaY = std::abs(toRow - fromRow);

        // Kr�l mo�e porusza� si� o jedno pole w ka�dym kierunku
        if (deltaX <= 1 && deltaY <= 1 && (deltaX != 0 || deltaY != 0)) {
            // Sprawdzenie, czy na docelowej pozycji znajduje si� wrogi kr�l
            if (board[toRow][toCol] == enemyKingValue) {
                return true; // Znaleziono wrogiego kr�la w odleg�o�ci jednego pola
            }
        }

        return false; // Wrogi kr�l nie znajduje si� w odleg�o�ci jednego pola
    }




    bool canCapturePiece(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], int targetPieceValue, bool isWhite) const override {
        // Najpierw sprawdzamy, czy kr�l mo�e si� przemie�ci� na docelowe pole
        if (!canMove(fromRow, fromCol, toRow, toCol, board, isWhite)) {
           return false;

        }

        return false;
    }



};

