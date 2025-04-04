#include "King.h"
#include "ChessBoard.h"

King::King(PieceColor color) : Piece(PieceType::KING, color) {
}

std::vector<sf::Vector2i> King::getPossibleMoves(int row, int col, const ChessBoard& board) const {
    std::vector<sf::Vector2i> moves;

    // Król mo¿e poruszaæ siê o jedno pole w dowolnym kierunku
    for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
            // Pomiñ aktualn¹ pozycjê
            if (r == 0 && c == 0) continue;

            int newRow = row + r;
            int newCol = col + c;

            // SprawdŸ, czy pozycja jest na planszy
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                const Piece* target = board.getPieceAt(newRow, newCol);
                if (target->isEmpty() || target->getColor() != color) {
                    moves.push_back(sf::Vector2i(newRow, newCol));
                }
            }
        }
    }

    // Tutaj mo¿na dodaæ roszadê (castling)
    // Wymaga to sprawdzenia, czy król i wie¿a nie rusza³y siê jeszcze
    // oraz czy pola miêdzy nimi s¹ puste

    return moves;
}