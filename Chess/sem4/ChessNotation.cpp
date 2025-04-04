//#include "ChessNotation.h"
//#include <cctype>
//#include <sstream>
//#include <iostream>
//#include <cmath>
//
//// Inicjalizacja statycznej tablicy z symbolami figur
//const char ChessNotation::pieceSymbols[6] = { 'R', 'N', 'B', 'Q', 'K', 'P' };
//
//ChessNotation::ChessNotation(const Board& board) : board(board) {
//}
//
//char ChessNotation::getPieceSymbol(int pieceValue) const {
//    int absValue = std::abs(pieceValue);
//    if (absValue >= 1 && absValue <= 6) {
//        return pieceSymbols[absValue - 1];
//    }
//    return ' ';
//}
//
//std::string ChessNotation::getSquareName(int row, int col) const {
//    // Kolumny s¹ oznaczane literami a-h
//    char colChar = 'a' + col;
//
//    // Wiersze s¹ numerowane od do³u, od 1 do 8
//    char rowChar = '8' - row;
//
//    std::string squareName;
//    squareName += colChar;
//    squareName += rowChar;
//
//    return squareName;
//}
//
//std::string ChessNotation::moveToAlgebraic(const ChessMove& move) const {
//    // Implementacja domyœlnie u¿ywa notacji d³ugiej
//    if (move.notation.empty()) {
//        return toLongNotation(move.fromRow, move.fromCol, move.toRow, move.toCol);
//    }
//    return move.notation;
//}
//
//ChessMove ChessNotation::algebraicToMove(const std::string& notation, bool isWhiteTurn) const {
//    ChessMove move;
//    move.notation = notation;
//
//    // Przyk³adowa implementacja konwersji notacji na ruch
//    // Dla prostoty obs³ugujemy tylko d³ugi format (np. e2-e4)
//
//    if (notation.length() >= 5 && notation[2] == '-') {
//        // Format: e2-e4
//        char fromCol = notation[0];
//        char fromRow = notation[1];
//        char toCol = notation[3];
//        char toRow = notation[4];
//
//        // Konwersja wspó³rzêdnych
//        move.fromCol = fromCol - 'a';
//        move.fromRow = '8' - fromRow;
//        move.toCol = toCol - 'a';
//        move.toRow = '8' - toRow;
//
//        // Pobierz informacje o figurach
//        move.movedPiece = board.getPieceAt(move.fromRow, move.fromCol);
//        move.capturedPiece = board.getPieceAt(move.toRow, move.toCol);
//    }
//    // Tutaj mo¿na dodaæ obs³ugê innych formatów
//
//    return move;
//}
//
//std::string ChessNotation::toLongNotation(int fromRow, int fromCol, int toRow, int toCol) const {
//    std::string notation;
//
//    // Format: e2-e4
//    notation += getSquareName(fromRow, fromCol);
//    notation += "-";
//    notation += getSquareName(toRow, toCol);
//
//    return notation;
//}
//
//std::string ChessNotation::toShortNotation(int fromRow, int fromCol, int toRow, int toCol, bool isCheck, bool isCheckmate) const {
//    std::string notation;
//
//    // Pobierz informacje o figurach
//    int piece = board.getPieceAt(fromRow, fromCol);
//    int capturedPiece = board.getPieceAt(toRow, toCol);
//
//    // Dodaj symbol figury (oprócz pionka)
//    if (std::abs(piece) != 6) {
//        notation += getPieceSymbol(piece);
//    }
//
//    // Dodaj oznaczenie bicia
//    if (capturedPiece != 0) {
//        // Dla pionka dodaj kolumnê, z której bije
//        if (std::abs(piece) == 6) {
//            notation += 'a' + fromCol;
//        }
//        notation += 'x';
//    }
//
//    // Dodaj pole docelowe
//    notation += getSquareName(toRow, toCol);
//
//    // Dodaj symbol szacha lub mata
//    if (isCheckmate) {
//        notation += "#";
//    }
//    else if (isCheck) {
//        notation += "+";
//    }
//
//    return notation;
//}