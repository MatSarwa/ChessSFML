//#include "MoveValidator.h"
//#include "Piece.h"
//#include "Pawn.h"
//#include "Rook.h"
//#include "Knight.h"
//#include "Bishop.h"
//#include "Queen.h"
//#include "King.h"
//#include <cmath>
//
//MoveValidator::MoveValidator(const Board& board)
//    : board(board), whiteKingRow(-1), whiteKingCol(-1), blackKingRow(-1), blackKingCol(-1) {
//    updateKingsPositions();
//}
//
//void MoveValidator::updateKingsPositions() {
//    // ZnajdŸ pozycje obu królów na planszy
//    const int(*boardArray)[8] = board.getBoard();
//
//    for (int i = 0; i < 8; ++i) {
//        for (int j = 0; j < 8; ++j) {
//            if (boardArray[i][j] == 4) { // Bia³y król
//                whiteKingRow = i;
//                whiteKingCol = j;
//            }
//            else if (boardArray[i][j] == -4) { // Czarny król
//                blackKingRow = i;
//                blackKingCol = j;
//            }
//        }
//    }
//}
//
//bool MoveValidator::isPieceOwner(int row, int col, bool isWhite) const {
//    int piece = board.getPieceAt(row, col);
//    return (piece != 0 && ((piece > 0) == isWhite));
//}
//
//bool MoveValidator::isSquareEmpty(int row, int col) const {
//    return board.getPieceAt(row, col) == 0;
//}
//
//bool MoveValidator::isOpponentPiece(int row, int col, bool isWhite) const {
//    int piece = board.getPieceAt(row, col);
//    return (piece != 0 && ((piece > 0) != isWhite));
//}
//
//bool MoveValidator::isValidMove(int fromRow, int fromCol, int toRow, int toCol, bool isWhite) const {
//    // SprawdŸ czy figury znajduj¹ siê na planszy
//    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
//        toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) {
//        return false;
//    }
//
//    // SprawdŸ czy na polu Ÿród³owym jest figura w³aœciwego koloru
//    if (!isPieceOwner(fromRow, fromCol, isWhite)) {
//        return false;
//    }
//
//    // SprawdŸ czy pole docelowe nie zawiera figury tego samego koloru
//    if (isPieceOwner(toRow, toCol, isWhite)) {
//        return false;
//    }
//
//    // Pobierz figurê i sprawdŸ czy mo¿e wykonaæ taki ruch
//    int pieceValue = board.getPieceAt(fromRow, fromCol);
//    Piece* piece = nullptr;
//
//    switch (std::abs(pieceValue)) {
//    case 1: piece = new Rook(); break;
//    case 2: piece = new Knight(); break;
//    case 3: piece = new Bishop(); break;
//    case 4: piece = new King(); break;
//    case 5: piece = new Queen(); break;
//    case 6: piece = new Pawn(); break;
//    default: return false;
//    }
//
//    bool isValid = false;
//    const int(*boardArray)[8] = board.getBoard();
//
//    // SprawdŸ czy ruch jest dozwolony dla danej figury
//    if (isSquareEmpty(toRow, toCol)) {
//        isValid = piece->canMove(fromRow, fromCol, toRow, toCol, boardArray, isWhite);
//    }
//    else {
//        isValid = piece->canCapture(fromRow, fromCol, toRow, toCol, boardArray, isWhite);
//    }
//
//    delete piece;
//
//    // Jeœli ruch jest dozwolony, sprawdŸ czy po ruchu król nie bêdzie szachowany
//    if (isValid) {
//        return !isKingInCheckAfterMove(fromRow, fromCol, toRow, toCol, isWhite);
//    }
//
//    return false;
//}
//
//bool MoveValidator::isKingInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol, bool isWhite) const {
//    // Stwórz kopiê planszy
//    int tempBoard[8][8];
//    const int(*boardArray)[8] = board.getBoard();
//
//    for (int i = 0; i < 8; ++i) {
//        for (int j = 0; j < 8; ++j) {
//            tempBoard[i][j] = boardArray[i][j];
//        }
//    }
//
//    // Wykonaj ruch na kopii
//    int piece = tempBoard[fromRow][fromCol];
//    tempBoard[toRow][toCol] = piece;
//    tempBoard[fromRow][fromCol] = 0;
//
//    // Aktualizuj pozycjê króla, jeœli to król siê poruszy³
//    int kingRow = isWhite ? whiteKingRow : blackKingRow;
//    int kingCol = isWhite ? whiteKingCol : blackKingCol;
//
//    if (std::abs(piece) == 4) { // Jeœli to król
//        kingRow = toRow;
//        kingCol = toCol;
//    }
//
//    // SprawdŸ czy jakakolwiek figura przeciwnika mo¿e atakowaæ króla
//    for (int i = 0; i < 8; ++i) {
//        for (int j = 0; j < 8; ++j) {
//            // Jeœli pole zawiera figurê przeciwnika
//            if (tempBoard[i][j] != 0 && ((tempBoard[i][j] > 0) != isWhite)) {
//                Piece* opponentPiece = nullptr;
//
//                switch (std::abs(tempBoard[i][j])) {
//                case 1: opponentPiece = new Rook(); break;
//                case 2: opponentPiece = new Knight(); break;
//                case 3: opponentPiece = new Bishop(); break;
//                case 4: opponentPiece = new King(); break;
//                case 5: opponentPiece = new Queen(); break;
//                case 6: opponentPiece = new Pawn(); break;
//                default: continue;
//                }
//
//                // SprawdŸ czy figura przeciwnika mo¿e zbiæ króla
//                bool canAttack = opponentPiece->canCapture(i, j, kingRow, kingCol, tempBoard, !isWhite);
//                delete opponentPiece;
//
//                if (canAttack) {
//                    return true; // Król bêdzie szachowany
//                }
//            }
//        }
//    }
//
//    return false; // Król nie bêdzie szachowany
//}
//
//std::vector<sf::Vector2i> MoveValidator::getPossibleMoves(int row, int col, bool isWhite) const {
//    std::vector<sf::Vector2i> moves;
//
//    // SprawdŸ czy na danym polu jest figura w³aœciwego koloru
//    if (!isPieceOwner(row, col, isWhite)) {
//        return moves;
//    }
//
//    // SprawdŸ wszystkie mo¿liwe pola docelowe
//    for (int toRow = 0; toRow < 8; ++toRow) {
//        for (int toCol = 0; toCol < 8; ++toCol) {
//            if (isValidMove(row, col, toRow, toCol, isWhite)) {
//                moves.push_back(sf::Vector2i(toRow, toCol));
//            }
//        }
//    }
//
//    return moves;
//}
//
//bool MoveValidator::isKingInCheck(bool isWhite) const {
//    int kingRow = isWhite ? whiteKingRow : blackKingRow;
//    int kingCol = isWhite ? whiteKingCol : blackKingCol;
//
//    // SprawdŸ czy jakakolwiek figura przeciwnika mo¿e zbiæ króla
//    for (int i = 0; i < 8; ++i) {
//        for (int j = 0; j < 8; ++j) {
//            if (isOpponentPiece(i, j, isWhite)) {
//                int pieceValue = board.getPieceAt(i, j);
//                Piece* piece = nullptr;
//
//                switch (std::abs(pieceValue)) {
//                case 1: piece = new Rook(); break;
//                case 2: piece = new Knight(); break;
//                case 3: piece = new Bishop(); break;
//                case 4: piece = new King(); break;
//                case 5: piece = new Queen(); break;
//                case 6: piece = new Pawn(); break;
//                default: continue;
//                }
//
//                const int(*boardArray)[8] = board.getBoard();
//                bool canAttack = piece->canCapture(i, j, kingRow, kingCol, boardArray, !isWhite);
//                delete piece;
//
//                if (canAttack) {
//                    return true; // Król jest szachowany
//                }
//            }
//        }
//    }
//
//    return false; // Król nie jest szachowany
//}
//
//std::vector<sf::Vector2i> MoveValidator::getAttackingPieces(bool isWhite) const {
//    std::vector<sf::Vector2i> attackers;
//    int kingRow = isWhite ? whiteKingRow : blackKingRow;
//    int kingCol = isWhite ? whiteKingCol : blackKingCol;
//
//    for (int i = 0; i < 8; ++i) {
//        for (int j = 0; j < 8; ++j) {
//            if (isOpponentPiece(i, j, isWhite)) {
//                int pieceValue = board.getPieceAt(i, j);
//                Piece* piece = nullptr;
//
//                switch (std::abs(pieceValue)) {
//                case 1: piece = new Rook(); break;
//                case 2: piece = new Knight(); break;
//                case 3: piece = new Bishop(); break;
//                case 4: piece = new King(); break;
//                case 5: piece = new Queen(); break;
//                case 6: piece = new Pawn(); break;
//                default: continue;
//                }
//
//                const int(*boardArray)[8] = board.getBoard();
//                bool canAttack = piece->canCapture(i, j, kingRow, kingCol, boardArray, !isWhite);
//                delete piece;
//
//                if (canAttack) {
//                    attackers.push_back(sf::Vector2i(i, j));
//                }
//            }
//        }
//    }
//
//    return attackers;
//}
//
//bool MoveValidator::canKingEscapeCheck(bool isWhite) const {
//    int kingRow = isWhite ? whiteKingRow : blackKingRow;
//    int kingCol = isWhite ? whiteKingCol : blackKingCol;
//
//    // SprawdŸ wszystkie mo¿liwe ruchy króla
//    for (int i = -1; i <= 1; ++i) {
//        for (int j = -1; j <= 1; ++j) {
//            if (i == 0 && j == 0) continue; // Pomiñ aktualn¹ pozycjê
//
//            int newRow = kingRow + i;
//            int newCol = kingCol + j;
//
//            // SprawdŸ czy nowa pozycja jest na planszy
//            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
//                // SprawdŸ czy król mo¿e siê tam ruszyæ
//                if (isValidMove(kingRow, kingCol, newRow, newCol, isWhite)) {
//                    return true; // Król mo¿e uciec
//                }
//            }
//        }
//    }
//
//    return false; // Król nie mo¿e uciec
//}
//
//bool MoveValidator::canBlockCheck(bool isWhite) const {
//    // Pobierz atakuj¹ce figury
//    std::vector<sf::Vector2i> attackers = getAttackingPieces(isWhite);
//
//    // Jeœli jest wiêcej ni¿ jeden atakuj¹cy, nie mo¿na zablokowaæ
//    if (attackers.size() != 1) {
//        return false;
//    }
//
//    sf::Vector2i attacker = attackers[0];
//    int kingRow = isWhite ? whiteKingRow : blackKingRow;
//    int kingCol = isWhite ? whiteKingCol : blackKingCol;
//
//    // Jeœli atakuj¹cy to skoczek, nie mo¿na go zablokowaæ
//    int attackerPiece = board.getPieceAt(attacker.x, attacker.y);
//    if (std::abs(attackerPiece) == 2) { // Skoczek
//        return false;
//    }
//
//    // ZnajdŸ pola miêdzy królem a atakuj¹cym
//    std::vector<sf::Vector2i> blockingSquares;
//
//    // Ró¿nica wspó³rzêdnych
//    int rowDiff = kingRow - attacker.x;
//    int colDiff = kingCol - attacker.y;
//
//    // Wyznacz kierunek ataku
//    int rowDir = 0;
//    int colDir = 0;
//
//    if (rowDiff != 0) rowDir = rowDiff > 0 ? 1 : -1;
//    if (colDiff != 0) colDir = colDiff > 0 ? 1 : -1;
//
//    // Zbierz pola pomiêdzy królem a atakuj¹cym
//    int row = attacker.x + rowDir;
//    int col = attacker.y + colDir;
//
//    while (row != kingRow || col != kingCol) {
//        blockingSquares.push_back(sf::Vector2i(row, col));
//        row += rowDir;
//        col += colDir;
//    }
//
//    // SprawdŸ, czy którakolwiek z figur mo¿e zablokowaæ atak
//    for (int i = 0; i < 8; ++i) {
//        for (int j = 0; j < 8; ++j) {
//            if (isPieceOwner(i, j, isWhite) && board.getPieceAt(i, j) != 4) { // Nie sprawdzaj króla
//                for (const auto& square : blockingSquares) {
//                    if (isValidMove(i, j, square.x, square.y, isWhite)) {
//                        return true; // Mo¿na zablokowaæ atak
//                    }
//                }
//            }
//        }
//    }
//
//    return false; // Nie mo¿na zablokowaæ ataku
//}
//
//bool MoveValidator::canCaptureAttacker(bool isWhite) const {
//    // Pobierz atakuj¹ce figury
//    std::vector<sf::Vector2i> attackers = getAttackingPieces(isWhite);
//
//    // SprawdŸ, czy mo¿na zbiæ któr¹kolwiek z atakuj¹cych figur
//    for (const auto& attacker : attackers) {
//        for (int i = 0; i < 8; ++i) {
//            for (int j = 0; j < 8; ++j) {
//                if (isPieceOwner(i, j, isWhite)) {
//                    if (isValidMove(i, j, attacker.x, attacker.y, isWhite)) {
//                        return true; // Mo¿na zbiæ atakuj¹cego
//                    }
//                }
//            }
//        }
//    }
//
//    return false; // Nie mo¿na zbiæ ¿adnego atakuj¹cego
//}
//
//bool MoveValidator::isCheckmate(bool isWhite) const {
//    // SprawdŸ czy król jest w szachu
//    if (!isKingInCheck(isWhite)) {
//        return false;
//    }
//
//    // SprawdŸ czy król mo¿e uciec
//    if (canKingEscapeCheck(isWhite)) {
//        return false;
//    }
//
//    // SprawdŸ czy mo¿na zablokowaæ lub zbiæ atakuj¹cego
//    if (canBlockCheck(isWhite) || canCaptureAttacker(isWhite)) {
//        return false;
//    }
//
//    return true; // Szach-mat
//}
//
//bool MoveValidator::isStalemate(bool isWhite) const {
//    // Jeœli król jest w szachu, to nie ma pata
//    if (isKingInCheck(isWhite)) {
//        return false;
//    }
//
//    // SprawdŸ czy którakolwiek figura mo¿e wykonaæ ruch
//    for (int i = 0; i < 8; ++i) {
//        for (int j = 0; j < 8; ++j) {
//            if (isPieceOwner(i, j, isWhite)) {
//                std::vector<sf::Vector2i> moves = getPossibleMoves(i, j, isWhite);
//                if (!moves.empty()) {
//                    return false; // Jest przynajmniej jeden mo¿liwy ruch
//                }
//            }
//        }
//    }
//
//    return true; // Pat - brak mo¿liwych ruchów, król nie jest szachowany
//}