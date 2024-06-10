#pragma once
#include <iostream>

class ChessBoard {
private:
    int board[8][8];

public:
    // Constructor
    ChessBoard() {
        // Initialize the board with the given values
        int initialBoard[8][8] = {
            {-1,-2,-3,-4,-5,-3,-2,-1},
            {-6,-6,-6,-6,-6,-6,-6,-6},
            { 0, 0, 0, 0, 0, 0, 0, 0},
            { 0, 0, 0, 0, 0, 0, 0, 0},
            { 0, 0, 0, 0, 0, 0, 0, 0},
            { 0, 0, 0, 0, 0, 0, 0, 0},
            { 6, 6, 6, 6, 6, 6, 6, 6},
            { 1, 2, 3, 4, 5, 3, 2, 1}
        };

        // Copy initialBoard to board
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = initialBoard[i][j];
            }
        }
    }

    // Getter for the board array
    const int(*getBoard() const)[8] {
        return board;
        }

        // Metoda do pobierania wartoœci na planszy dla danego pola
            int getPieceAt(int row, int col) const {
            return board[row][col];
        }

        // Metoda do przesuwania pionka na planszy
        void movePiece(int fromRow, int fromCol, int toRow, int toCol) {
            int piece = board[fromRow][fromCol];

            if (piece != 0) {
                board[toRow][toCol] = piece;
                board[fromRow][fromCol] = 0;
            }
        }


};
