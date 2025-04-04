#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Piece.h"

class ChessBoard;

class Queen : public Piece {
public:
    Queen(PieceColor color);
    std::vector<sf::Vector2i> getPossibleMoves(int row, int col, const ChessBoard& board) const override;
};