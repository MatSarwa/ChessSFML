#include "Bishop.h"
#include "ChessBoard.h"

Bishop::Bishop(PieceColor color) : Piece(PieceType::BISHOP, color) {
}

std::vector<sf::Vector2i> Bishop::getPossibleMoves(int row, int col, const ChessBoard& board) const {
    std::vector<sf::Vector2i> moves;

    // Kierunki ruchu goñca: ukoœnie w 4 strony
    const int directions[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

    for (auto& dir : directions) {
        int r = row;
        int c = col;

        while (true) {
            r += dir[0];
            c += dir[1];

            // SprawdŸ, czy pozycja jest na planszy
            if (r < 0 || r >= 8 || c < 0 || c >= 8) {
                break;
            }

            const Piece* target = board.getPieceAt(r, c);
            if (target->isEmpty()) {
                moves.push_back(sf::Vector2i(r, c));
            }
            else {
                if (target->getColor() != color) {
                    moves.push_back(sf::Vector2i(r, c));
                }
                break;
            }
        }
    }

    return moves;
}