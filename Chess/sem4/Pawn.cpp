#include "Pawn.h"
#include "ChessBoard.h"

Pawn::Pawn(PieceColor color) : Piece(PieceType::PAWN, color) {
}

std::vector<sf::Vector2i> Pawn::getPossibleMoves(int row, int col, const ChessBoard& board) const {
    std::vector<sf::Vector2i> moves;
    int direction = isWhite() ? -1 : 1;  // Bia�y pionek idzie w g�r� (-1), czarny w d� (1)

    // Ruch do przodu
    if (board.getPieceAt(row + direction, col)->isEmpty()) {
        moves.push_back(sf::Vector2i(row + direction, col));

        // Podw�jny ruch z pozycji startowej
        if ((isWhite() && row == 6) || (isBlack() && row == 1)) {
            if (board.getPieceAt(row + 2 * direction, col)->isEmpty()) {
                moves.push_back(sf::Vector2i(row + 2 * direction, col));
            }
        }
    }

    // Bicie na ukos
    for (int offset : {-1, 1}) {
        int newCol = col + offset;
        if (newCol >= 0 && newCol < 8) {
            const Piece* target = board.getPieceAt(row + direction, newCol);
            if (!target->isEmpty() && target->getColor() != color) {
                moves.push_back(sf::Vector2i(row + direction, newCol));
            }
        }
    }

    // Tutaj mo�na doda� bicie w przelocie (en passant)

    return moves;
}