//#pragma once
//#include <string>
//#include <vector>
//#include "Board.h"
//
//// Struktura reprezentuj¹ca ruch w szachach
//struct ChessMove {
//    int fromRow;
//    int fromCol;
//    int toRow;
//    int toCol;
//    int movedPiece;      // Wartoœæ przesuwanej figury
//    int capturedPiece;   // Wartoœæ zbitej figury (0 jeœli brak)
//    bool isCheck;        // Czy ruch daje szacha
//    bool isCheckmate;    // Czy ruch daje mata
//    bool isPromotion;    // Czy jest to promocja pionka
//    int promotionPiece;  // Typ figury po promocji
//    std::string notation; // Notacja algebraiczna ruchu
//
//    // Konstruktor domyœlny - inicjalizuje wartoœci
//    ChessMove() : fromRow(-1), fromCol(-1), toRow(-1), toCol(-1),
//        movedPiece(0), capturedPiece(0),
//        isCheck(false), isCheckmate(false),
//        isPromotion(false), promotionPiece(0) {
//    }
//};
//
//// Klasa odpowiedzialna za konwersjê ruchów na notacjê algebraiczn¹ i odwrotnie
//class ChessNotation {
//private:
//    const Board& board;
//
//    // Symbole figur w notacji
//    static const char pieceSymbols[6];
//
//    // Pomocnicze metody
//    char getPieceSymbol(int pieceValue) const;
//    std::string getSquareName(int row, int col) const;
//
//public:
//    ChessNotation(const Board& board);
//
//    // Konwersja ruchu na notacjê algebraiczn¹
//    std::string moveToAlgebraic(const ChessMove& move) const;
//
//    // Konwersja notacji algebraicznej na ruch
//    ChessMove algebraicToMove(const std::string& notation, bool isWhiteTurn) const;
//
//    // Generowanie d³ugiej notacji algebraicznej (np. e2-e4)
//    std::string toLongNotation(int fromRow, int fromCol, int toRow, int toCol) const;
//
//    // Generowanie krótkiej notacji algebraicznej (np. Nf3)
//    std::string toShortNotation(int fromRow, int fromCol, int toRow, int toCol, bool isCheck, bool isCheckmate) const;
//};