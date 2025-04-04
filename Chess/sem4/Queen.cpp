#include "Queen.h"
#include "ChessBoard.h"

Queen::Queen(PieceColor color) : Piece(PieceType::QUEEN, color) {
}

std::vector<sf::Vector2i> Queen::getPossibleMoves(int row, int col, const ChessBoard& board) const {
    std::vector<sf::Vector2i> moves;

    // Kierunki ruchu królowej: po³¹czenie ruchów wie¿y i goñca (8 kierunków)
    const int directions[8][2] = {
        {-1, 0}, {0, 1}, {1, 0}, {0, -1}, // jak wie¿a
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1} // jak goniec
    };

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