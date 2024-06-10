#pragma once

class Piece {
public:
    virtual bool canMove(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const = 0;
    virtual bool canCapture(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const = 0;
    virtual bool canCaptureKing(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], bool isWhite) const = 0;
    virtual bool canCapturePiece(int fromRow, int fromCol, int toRow, int toCol, const int board[8][8], int targetPieceValue, bool isWhite) const = 0;
    virtual ~Piece() {}
};
