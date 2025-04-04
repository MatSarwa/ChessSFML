//#pragma once
//#include "Board.h"
//#include "MoveValidator.h"
//#include <vector>
//#include <string>
//
//// Enumy dla stanu gry
//enum class GameResult {
//    ONGOING,
//    WHITE_WIN,
//    BLACK_WIN,
//    DRAW
//};
//
//enum class GamePhase {
//    OPENING,
//    MIDDLEGAME,
//    ENDGAME
//};
//
//// Klasa zarz¹dzaj¹ca stanem gry
//class GameState {
//private:
//    // Referencje do innych komponentów
//    const Board& board;
//    MoveValidator& validator;
//
//    // Wewnêtrzny stan gry
//    bool whiteTurn;
//    GameResult result;
//    GamePhase phase;
//    std::string resultReason;
//
//    // Liczniki ruchów
//    int halfMoveCount;   // Licznik pó³ruchów (resetowany po biciu lub ruchu pionkiem)
//    int fullMoveCount;   // Licznik pe³nych ruchów
//
//    // Pola do zasady 50 ruchów i zasady trzykrotnego powtórzenia
//    int movesSinceCaptureOrPawn;
//    std::vector<std::string> positionHistory;
//
//    // Statystyki gry
//    int whiteCaptures;
//    int blackCaptures;
//
//    // Prywatne metody pomocnicze
//    void updateGamePhase();
//    void checkDrawConditions();
//    bool isInsufficientMaterial() const;
//    bool isThreefoldRepetition() const;
//
//public:
//    GameState(const Board& board, MoveValidator& validator);
//
//    // Aktualizacja stanu gry
//    void update();
//
//    // Wykonanie ruchu
//    void makeMove(int fromRow, int fromCol, int toRow, int toCol);
//
//    // Resetowanie stanu gry
//    void reset();
//
//    // Gettery stanu gry
//    bool isWhiteTurn() const { return whiteTurn; }
//    GameResult getResult() const { return result; }
//    GamePhase getPhase() const { return phase; }
//    std::string getResultReason() const { return resultReason; }
//    int getHalfMoveCount() const { return halfMoveCount; }
//    int getFullMoveCount() const { return fullMoveCount; }
//    int getWhiteCaptures() const { return whiteCaptures; }
//    int getBlackCaptures() const { return blackCaptures; }
//
//    // Zmiana aktywnego gracza
//    void toggleTurn();
//
//    // Sprawdzenie warunków koñca gry
//    bool isGameOver() const;
//
//    // Metody pomocnicze
//    int getMaterialBalance() const; // Ró¿nica materia³u (dodatnia dla bia³ych)
//};