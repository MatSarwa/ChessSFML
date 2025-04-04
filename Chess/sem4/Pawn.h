#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Piece.h"

class ChessBoard;

class Pawn : public Piece {
public:
    Pawn(PieceColor color);
    std::vector<sf::Vector2i> getPossibleMoves(int row, int col, const ChessBoard& board) const override;
};