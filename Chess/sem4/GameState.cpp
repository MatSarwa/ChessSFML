//#include "GameState.h"
//#include <algorithm>
//
//GameState::GameState(const Board& board, MoveValidator& validator)
//    : board(board), validator(validator), whiteTurn(true),
//    result(GameResult::ONGOING), phase(GamePhase::OPENING),
//    halfMoveCount(0), fullMoveCount(1),
//    movesSinceCaptureOrPawn(0),
//    whiteCaptures(0), blackCaptures(0)
//{
//    // Inicjalizacja zako�czona w li�cie inicjalizacyjnej
//}
//
//void GameState::update() {
//    // Sprawd� warunki ko�ca gry
//    if (validator.isCheckmate(whiteTurn)) {
//        result = whiteTurn ? GameResult::BLACK_WIN : GameResult::WHITE_WIN;
//        resultReason = whiteTurn ? "Czarny wygrywa (szach-mat)" : "Bia�y wygrywa (szach-mat)";
//        return;
//    }
//
//    if (validator.isStalemate(whiteTurn)) {
//        result = GameResult::DRAW;
//        resultReason = "Remis (pat)";
//        return;
//    }
//
//    // Sprawd� warunki remisu
//    checkDrawConditions();
//
//    // Zaktualizuj faz� gry
//    updateGamePhase();
//}
//
//void GameState::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
//    int piece = board.getPieceAt(fromRow, fromCol);
//    int capturedPiece = board.getPieceAt(toRow, toCol);
//
//    // Aktualizuj liczniki
//    fullMoveCount += (whiteTurn ? 0 : 1); // Zwi�ksz po ruchu czarnych
//
//    // Resetuj licznik p�-ruch�w przy biciu lub ruchu pionka
//    if (capturedPiece != 0 || std::abs(piece) == 6) {
//        movesSinceCaptureOrPawn = 0;
//
//        // Aktualizuj statystyki zbitych figur
//        if (capturedPiece != 0) {
//            if (whiteTurn) {
//                whiteCaptures++;
//            }
//            else {
//                blackCaptures++;
//            }
//        }
//    }
//    else {
//        movesSinceCaptureOrPawn++;
//    }
//
//    // Dodaj aktualn� pozycj� do historii (dla zasady trzykrotnego powt�rzenia)
//    positionHistory.push_back(board.toFEN());
//
//    // Zmie� tur�
//    whiteTurn = !whiteTurn;
//
//    // Aktualizuj stan gry
//    update();
//}
//
//void GameState::reset() {
//    whiteTurn = true;
//    result = GameResult::ONGOING;
//    phase = GamePhase::OPENING;
//    resultReason = "";
//    halfMoveCount = 0;
//    fullMoveCount = 1;
//    movesSinceCaptureOrPawn = 0;
//    positionHistory.clear();
//    whiteCaptures = 0;
//    blackCaptures = 0;
//}
//
//void GameState::toggleTurn() {
//    whiteTurn = !whiteTurn;
//}
//
//bool GameState::isGameOver() const {
//    return result != GameResult::ONGOING;
//}
//
//void GameState::updateGamePhase() {
//    // Uproszczone okre�lenie fazy gry na podstawie liczby ruch�w
//    // i ilo�ci figur na planszy
//
//    if (fullMoveCount <= 10) {
//        phase = GamePhase::OPENING;
//    }
//    else if (getMaterialBalance() < 20) {
//        phase = GamePhase::ENDGAME;
//    }
//    else {
//        phase = GamePhase::MIDDLEGAME;
//    }
//}
//
//void GameState::checkDrawConditions() {
//    // Sprawd� zasad� 50 ruch�w
//    if (movesSinceCaptureOrPawn >= 50) {
//        result = GameResult::DRAW;
//        resultReason = "Remis (zasada 50 ruch�w)";
//        return;
//    }
//
//    // Sprawd� zasad� trzykrotnego powt�rzenia
//    if (isThreefoldRepetition()) {
//        result = GameResult::DRAW;
//        resultReason = "Remis (trzykrotne powt�rzenie)";
//        return;
//    }
//
//    // Sprawd� niewystarczaj�cy materia�
//    if (isInsufficientMaterial()) {
//        result = GameResult::DRAW;
//        resultReason = "Remis (niewystarczaj�cy materia�)";
//        return;
//    }
//}
//
//bool GameState::isThreefoldRepetition() const {
//    // Sprawd� czy aktualna pozycja wyst�pi�a ju� trzykrotnie
//    if (positionHistory.empty()) {
//        return false;
//    }
//
//    std::string currentPosition = positionHistory.back();
//    int count = 0;
//
//    for (const auto& position : positionHistory) {
//        if (position == currentPosition) {
//            count++;
//        }
//    }
//
//    return count >= 3;
//}
//
//bool GameState::isInsufficientMaterial() const {
//    // Uproszczona implementacja sprawdzaj�ca tylko najbardziej podstawowe przypadki
//
//    int pieceCount = 0;
//    bool hasWhiteBishop = false;
//    bool hasBlackBishop = false;
//    bool hasWhiteKnight = false;
//    bool hasBlackKnight = false;
//
//    for (int i = 0; i < 8; i++) {
//        for (int j = 0; j < 8; j++) {
//            int piece = board.getPieceAt(i, j);
//            if (piece != 0 && std::abs(piece) != 4) { // Pomi� kr�le
//                pieceCount++;
//
//                // Sprawd� go�ce i skoczki
//                if (piece == 3) hasWhiteBishop = true;
//                if (piece == -3) hasBlackBishop = true;
//                if (piece == 2) hasWhiteKnight = true;
//                if (piece == -2) hasBlackKnight = true;
//
//                // Je�li jest pionek, wie�a lub kr�lowa, to na pewno nie jest niewystarczaj�cy materia�
//                if (std::abs(piece) == 1 || std::abs(piece) == 5 || std::abs(piece) == 6) {
//                    return false;
//                }
//            }
//        }
//    }
//
//    // Kr�l przeciwko kr�lowi
//    if (pieceCount == 0) {
//        return true;
//    }
//
//    // Kr�l i goniec przeciwko kr�lowi
//    if (pieceCount == 1 && (hasWhiteBishop || hasBlackBishop)) {
//        return true;
//    }
//
//    // Kr�l i skoczek przeciwko kr�lowi
//    if (pieceCount == 1 && (hasWhiteKnight || hasBlackKnight)) {
//        return true;
//    }
//
//    // Kr�l i dwa skoczki przeciwko samemu kr�lowi - technicznie jest mo�liwy mat,
//    // ale praktycznie niemo�liwy bez b��du przeciwnika
//    if (pieceCount == 2 && hasWhiteKnight && hasBlackKnight &&
//        !hasWhiteBishop && !hasBlackBishop) {
//        return true;
//    }
//
//    return false;
//}
//
//int GameState::getMaterialBalance() const {
//    // Oblicz r�nic� materia�u (warto�� dodatnia oznacza przewag� bia�ych)
//    int balance = 0;
//
//    // Warto�ci figur
//    const int pawnValue = 1;
//    const int knightValue = 3;
//    const int bishopValue = 3;
//    const int rookValue = 5;
//    const int queenValue = 9;
//
//    for (int i = 0; i < 8; i++) {
//        for (int j = 0; j < 8; j++) {
//            int piece = board.getPieceAt(i, j);
//            if (piece != 0) {
//                int pieceValue = 0;
//
//                switch (std::abs(piece)) {
//                case 1: pieceValue = rookValue; break;   // Wie�a
//                case 2: pieceValue = knightValue; break; // Skoczek
//                case 3: pieceValue = bishopValue; break; // Goniec
//                case 5: pieceValue = queenValue; break;  // Kr�lowa
//                case 6: pieceValue = pawnValue; break;   // Pionek
//                    // Kr�le s� pomijane w obliczeniach materia�u
//                }
//
//                if (piece > 0) {
//                    balance += pieceValue; // Bia�a figura
//                }
//                else {
//                    balance -= pieceValue; // Czarna figura
//                }
//            }
//        }
//    }
//
//    return balance;
//}