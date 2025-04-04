#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Piece.h"

class ChessBoard;

class Rook : public Piece {
public:
    Rook(PieceColor color);
    std::vector<sf::Vector2i> getPossibleMoves(int row, int col, const ChessBoard& board) const override;
};