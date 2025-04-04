#include "ChessBoard.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

ChessBoard::ChessBoard() : whiteKingMoved(false), blackKingMoved(false),
whiteRook1Moved(false), whiteRook2Moved(false),
blackRook1Moved(false), blackRook2Moved(false),
enPassantCol(-1) {
    resetBoard();
}

void ChessBoard::resetBoard() {
    // Czyszczenie planszy - u�ywamy nullptr zamiast pustych obiekt�w Piece
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col] = nullptr;
        }
    }

    // Ustawienie figur dla czarnych
    board[0][0] = std::make_unique<Rook>(PieceColor::BLACK);
    board[0][1] = std::make_unique<Knight>(PieceColor::BLACK);
    board[0][2] = std::make_unique<Bishop>(PieceColor::BLACK);
    board[0][3] = std::make_unique<Queen>(PieceColor::BLACK);
    board[0][4] = std::make_unique<King>(PieceColor::BLACK);
    board[0][5] = std::make_unique<Bishop>(PieceColor::BLACK);
    board[0][6] = std::make_unique<Knight>(PieceColor::BLACK);
    board[0][7] = std::make_unique<Rook>(PieceColor::BLACK);

    // Pionki czarne
    for (int col = 0; col < 8; col++) {
        board[1][col] = std::make_unique<Pawn>(PieceColor::BLACK);
    }

    // Pionki bia�e
    for (int col = 0; col < 8; col++) {
        board[6][col] = std::make_unique<Pawn>(PieceColor::WHITE);
    }

    // Ustawienie figur dla bia�ych
    board[7][0] = std::make_unique<Rook>(PieceColor::WHITE);
    board[7][1] = std::make_unique<Knight>(PieceColor::WHITE);
    board[7][2] = std::make_unique<Bishop>(PieceColor::WHITE);
    board[7][3] = std::make_unique<Queen>(PieceColor::WHITE);
    board[7][4] = std::make_unique<King>(PieceColor::WHITE);
    board[7][5] = std::make_unique<Bishop>(PieceColor::WHITE);
    board[7][6] = std::make_unique<Knight>(PieceColor::WHITE);
    board[7][7] = std::make_unique<Rook>(PieceColor::WHITE);

    // Reset stanu gry
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteRook1Moved = false;
    whiteRook2Moved = false;
    blackRook1Moved = false;
    blackRook2Moved = false;
    enPassantCol = -1;
}

const Piece* ChessBoard::getPieceAt(int row, int col) const {
    if (isValidPosition(row, col) && board[row][col]) {
        return board[row][col].get();
    }
    static EmptyPiece emptyPiece;
    return &emptyPiece;
}

Piece* ChessBoard::getPieceAt(int row, int col) {
    if (isValidPosition(row, col) && board[row][col]) {
        return board[row][col].get();
    }
    return nullptr;
}

void ChessBoard::setPieceAt(int row, int col, std::unique_ptr<Piece> piece) {
    if (isValidPosition(row, col)) {
        board[row][col] = std::move(piece);
    }
}

bool ChessBoard::isValidPosition(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool ChessBoard::isPieceOfCurrentPlayer(int row, int col, bool isWhiteTurn) const {
    const Piece* piece = getPieceAt(row, col);
    return piece && !piece->isEmpty() && ((isWhiteTurn && piece->isWhite()) || (!isWhiteTurn && piece->isBlack()));
}

std::vector<sf::Vector2i> ChessBoard::getPossibleMoves(int row, int col) const {
    const Piece* piece = getPieceAt(row, col);
    if (piece && !piece->isEmpty()) {
        return piece->getPossibleMoves(row, col, *this);
    }
    return {};
}

bool ChessBoard::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    // Sprawd� poprawno�� pozycji
    if (!isValidPosition(fromRow, fromCol) || !isValidPosition(toRow, toCol)) {
        return false;
    }

    Piece* piece = getPieceAt(fromRow, fromCol);
    if (!piece || piece->isEmpty()) {
        return false;
    }

    bool isWhiteTurn = piece->isWhite();

    // Sprawd�, czy ruch jest na li�cie mo�liwych ruch�w
    auto possibleMoves = getPossibleMoves(fromRow, fromCol);
    bool moveFound = false;
    for (const auto& move : possibleMoves) {
        if (move.x == toRow && move.y == toCol) {
            moveFound = true;
            break;
        }
    }

    if (!moveFound) {
        return false;
    }

    // Sprawd�, czy ruch nie nara�a kr�la na szach
    if (wouldMoveExposeKing(fromRow, fromCol, toRow, toCol, isWhiteTurn)) {
        return false;
    }

    // Wykonaj ruch
    auto movingPiece = std::move(board[fromRow][fromCol]);
    board[fromRow][fromCol] = nullptr;
    board[toRow][toCol] = std::move(movingPiece);

    // Oznacz, �e figura si� ruszy�a
    if (board[toRow][toCol]) {
        board[toRow][toCol]->setHasMoved(true);
    }

    // Aktualizacja stanu roszady
    if (board[toRow][toCol] && board[toRow][toCol]->getType() == PieceType::KING) {
        if (board[toRow][toCol]->isWhite()) {
            whiteKingMoved = true;
        }
        else {
            blackKingMoved = true;
        }
    }
    else if (board[toRow][toCol] && board[toRow][toCol]->getType() == PieceType::ROOK) {
        if (board[toRow][toCol]->isWhite()) {
            if (fromCol == 0) whiteRook1Moved = true;
            if (fromCol == 7) whiteRook2Moved = true;
        }
        else {
            if (fromCol == 0) blackRook1Moved = true;
            if (fromCol == 7) blackRook2Moved = true;
        }
    }

    // Aktualizacja bicia w przelocie
    if (board[toRow][toCol] && board[toRow][toCol]->getType() == PieceType::PAWN && abs(fromRow - toRow) == 2) {
        enPassantCol = fromCol;
    }
    else {
        enPassantCol = -1;
    }

    return true;
}

void ChessBoard::promotePawn(int row, int col, PieceType newType) {
    if (!isValidPosition(row, col)) {
        return;
    }

    Piece* piece = getPieceAt(row, col);
    if (!piece || piece->getType() != PieceType::PAWN) {
        return;
    }

    PieceColor color = piece->getColor();
    std::unique_ptr<Piece> newPiece;

    switch (newType) {
    case PieceType::QUEEN:
        newPiece = std::make_unique<Queen>(color);
        break;
    case PieceType::ROOK:
        newPiece = std::make_unique<Rook>(color);
        break;
    case PieceType::BISHOP:
        newPiece = std::make_unique<Bishop>(color);
        break;
    case PieceType::KNIGHT:
        newPiece = std::make_unique<Knight>(color);
        break;
    default:
        newPiece = std::make_unique<Queen>(color); // Domy�lnie promuj do kr�lowej
        break;
    }

    setPieceAt(row, col, std::move(newPiece));
}

bool ChessBoard::isKingInCheck(PieceColor kingColor) const {
    // Znajd� pozycj� kr�la
    int kingRow = -1, kingCol = -1;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            const Piece* piece = getPieceAt(row, col);
            if (piece && piece->getType() == PieceType::KING && piece->getColor() == kingColor) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }
        if (kingRow != -1) break;
    }

    if (kingRow == -1) return false; // Kr�l nie znaleziony

    // Sprawd� czy kt�rakolwiek figura przeciwnika mo�e zbi� kr�la
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            const Piece* piece = getPieceAt(row, col);
            if (piece && !piece->isEmpty() && piece->getColor() != kingColor) {
                auto moves = piece->getPossibleMoves(row, col, *this);
                for (const auto& move : moves) {
                    if (move.x == kingRow && move.y == kingCol) {
                        return true; // Kr�l jest szachowany
                    }
                }
            }
        }
    }

    return false; // Kr�l nie jest szachowany
}

bool ChessBoard::wouldMoveExposeKing(int fromRow, int fromCol, int toRow, int toCol, bool isWhiteTurn) const {
    // Nie mo�emy u�ywa� operator kopiowania, wi�c tworzymy now� plansz� i r�cznie kopiujemy figury
    ChessBoard* tempBoard = new ChessBoard();

    // Kopiujemy stan planszy (z wyj�tkiem figury, kt�r� przesuwamy)
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (r == fromRow && c == fromCol) continue;

            const Piece* piece = getPieceAt(r, c);
            if (piece && !piece->isEmpty()) {
                std::unique_ptr<Piece> pieceCopy;
                switch (piece->getType()) {
                case PieceType::PAWN: pieceCopy = std::make_unique<Pawn>(piece->getColor()); break;
                case PieceType::ROOK: pieceCopy = std::make_unique<Rook>(piece->getColor()); break;
                case PieceType::KNIGHT: pieceCopy = std::make_unique<Knight>(piece->getColor()); break;
                case PieceType::BISHOP: pieceCopy = std::make_unique<Bishop>(piece->getColor()); break;
                case PieceType::QUEEN: pieceCopy = std::make_unique<Queen>(piece->getColor()); break;
                case PieceType::KING: pieceCopy = std::make_unique<King>(piece->getColor()); break;
                default: continue;
                }
                tempBoard->setPieceAt(r, c, std::move(pieceCopy));
            }
        }
    }

    // Wykonujemy ruch
    const Piece* piece = getPieceAt(fromRow, fromCol);
    if (piece && !piece->isEmpty()) {
        std::unique_ptr<Piece> pieceCopy;
        switch (piece->getType()) {
        case PieceType::PAWN: pieceCopy = std::make_unique<Pawn>(piece->getColor()); break;
        case PieceType::ROOK: pieceCopy = std::make_unique<Rook>(piece->getColor()); break;
        case PieceType::KNIGHT: pieceCopy = std::make_unique<Knight>(piece->getColor()); break;
        case PieceType::BISHOP: pieceCopy = std::make_unique<Bishop>(piece->getColor()); break;
        case PieceType::QUEEN: pieceCopy = std::make_unique<Queen>(piece->getColor()); break;
        case PieceType::KING: pieceCopy = std::make_unique<King>(piece->getColor()); break;
        default: break;
        }

        if (pieceCopy) {
            tempBoard->setPieceAt(toRow, toCol, std::move(pieceCopy));
        }
    }

    // Sprawdzamy, czy kr�l jest szachowany
    bool result = tempBoard->isKingInCheck(isWhiteTurn ? PieceColor::WHITE : PieceColor::BLACK);

    delete tempBoard;
    return result;
}

bool ChessBoard::isCheckmate(bool isWhiteTurn) const {
    PieceColor kingColor = isWhiteTurn ? PieceColor::WHITE : PieceColor::BLACK;

    // Najpierw sprawd�, czy kr�l jest w szachu
    if (!isKingInCheck(kingColor)) {
        return false;
    }

    // Sprawd� wszystkie mo�liwe ruchy dla wszystkich figur gracza
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            const Piece* piece = getPieceAt(row, col);
            if (piece && !piece->isEmpty() && piece->getColor() == kingColor) {
                auto moves = getPossibleMoves(row, col);

                // Dla ka�dego mo�liwego ruchu sprawd�, czy nadal jest szach
                for (const auto& move : moves) {
                    if (!wouldMoveExposeKing(row, col, move.x, move.y, isWhiteTurn)) {
                        return false; // Znaleziono ruch, kt�ry mo�e uratowa� kr�la
                    }
                }
            }
        }
    }

    return true; // Nie ma mo�liwo�ci obrony - szach-mat
}

bool ChessBoard::isStalemate(bool isWhiteTurn) const {
    PieceColor kingColor = isWhiteTurn ? PieceColor::WHITE : PieceColor::BLACK;

    // Sprawd�, czy kr�l jest w szachu
    if (isKingInCheck(kingColor)) {
        return false;
    }

    // Sprawd�, czy gracz ma jakiekolwiek dozwolone ruchy
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            const Piece* piece = getPieceAt(row, col);
            if (piece && !piece->isEmpty() && piece->getColor() == kingColor) {
                auto moves = getPossibleMoves(row, col);

                for (const auto& move : moves) {
                    if (!wouldMoveExposeKing(row, col, move.x, move.y, isWhiteTurn)) {
                        return false; // Gracz ma dozwolony ruch
                    }
                }
            }
        }
    }

    return true; // Gracz nie ma dozwolonych ruch�w - pat
}

bool ChessBoard::isInCheck(bool isWhitePlayer) const {
    // Znajd� pozycj� kr�la gracza
    sf::Vector2i kingPos(-1, -1);

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            const Piece* piece = getPieceAt(row, col);
            if (piece && piece->getType() == PieceType::KING &&
                ((isWhitePlayer && piece->isWhite()) || (!isWhitePlayer && piece->isBlack()))) {
                kingPos = sf::Vector2i(row, col);
                break;
            }
        }
        if (kingPos.x != -1) break;
    }

    // Sprawd�, czy jakikolwiek przeciwny pionek mo�e zaatakowa� kr�la
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            const Piece* piece = getPieceAt(row, col);
            if (piece && !piece->isEmpty() &&
                ((isWhitePlayer && piece->isBlack()) || (!isWhitePlayer && piece->isWhite()))) {

                // Pobierz mo�liwe ruchy dla tej figury
                auto moves = getPossibleMoves(row, col);

                // Sprawd�, czy kt�rykolwiek z ruch�w mo�e zaatakowa� kr�la
                for (const auto& move : moves) {
                    if (move.x == kingPos.x && move.y == kingPos.y) {
                        return true; // Kr�l jest w szachu
                    }
                }
            }
        }
    }

    return false; // Kr�l nie jest w szachu
}