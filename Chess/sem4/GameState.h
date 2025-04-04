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
//// Klasa zarz�dzaj�ca stanem gry
//class GameState {
//private:
//    // Referencje do innych komponent�w
//    const Board& board;
//    MoveValidator& validator;
//
//    // Wewn�trzny stan gry
//    bool whiteTurn;
//    GameResult result;
//    GamePhase phase;
//    std::string resultReason;
//
//    // Liczniki ruch�w
//    int halfMoveCount;   // Licznik p�ruch�w (resetowany po biciu lub ruchu pionkiem)
//    int fullMoveCount;   // Licznik pe�nych ruch�w
//
//    // Pola do zasady 50 ruch�w i zasady trzykrotnego powt�rzenia
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
//    // Sprawdzenie warunk�w ko�ca gry
//    bool isGameOver() const;
//
//    // Metody pomocnicze
//    int getMaterialBalance() const; // R�nica materia�u (dodatnia dla bia�ych)
//};