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
//    // Inicjalizacja zakoñczona w liœcie inicjalizacyjnej
//}
//
//void GameState::update() {
//    // SprawdŸ warunki koñca gry
//    if (validator.isCheckmate(whiteTurn)) {
//        result = whiteTurn ? GameResult::BLACK_WIN : GameResult::WHITE_WIN;
//        resultReason = whiteTurn ? "Czarny wygrywa (szach-mat)" : "Bia³y wygrywa (szach-mat)";
//        return;
//    }
//
//    if (validator.isStalemate(whiteTurn)) {
//        result = GameResult::DRAW;
//        resultReason = "Remis (pat)";
//        return;
//    }
//
//    // SprawdŸ warunki remisu
//    checkDrawConditions();
//
//    // Zaktualizuj fazê gry
//    updateGamePhase();
//}
//
//void GameState::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
//    int piece = board.getPieceAt(fromRow, fromCol);
//    int capturedPiece = board.getPieceAt(toRow, toCol);
//
//    // Aktualizuj liczniki
//    fullMoveCount += (whiteTurn ? 0 : 1); // Zwiêksz po ruchu czarnych
//
//    // Resetuj licznik pó³-ruchów przy biciu lub ruchu pionka
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
//    // Dodaj aktualn¹ pozycjê do historii (dla zasady trzykrotnego powtórzenia)
//    positionHistory.push_back(board.toFEN());
//
//    // Zmieñ turê
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
//    // Uproszczone okreœlenie fazy gry na podstawie liczby ruchów
//    // i iloœci figur na planszy
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
//    // SprawdŸ zasadê 50 ruchów
//    if (movesSinceCaptureOrPawn >= 50) {
//        result = GameResult::DRAW;
//        resultReason = "Remis (zasada 50 ruchów)";
//        return;
//    }
//
//    // SprawdŸ zasadê trzykrotnego powtórzenia
//    if (isThreefoldRepetition()) {
//        result = GameResult::DRAW;
//        resultReason = "Remis (trzykrotne powtórzenie)";
//        return;
//    }
//
//    // SprawdŸ niewystarczaj¹cy materia³
//    if (isInsufficientMaterial()) {
//        result = GameResult::DRAW;
//        resultReason = "Remis (niewystarczaj¹cy materia³)";
//        return;
//    }
//}
//
//bool GameState::isThreefoldRepetition() const {
//    // SprawdŸ czy aktualna pozycja wyst¹pi³a ju¿ trzykrotnie
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
//    // Uproszczona implementacja sprawdzaj¹ca tylko najbardziej podstawowe przypadki
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
//            if (piece != 0 && std::abs(piece) != 4) { // Pomiñ króle
//                pieceCount++;
//
//                // SprawdŸ goñce i skoczki
//                if (piece == 3) hasWhiteBishop = true;
//                if (piece == -3) hasBlackBishop = true;
//                if (piece == 2) hasWhiteKnight = true;
//                if (piece == -2) hasBlackKnight = true;
//
//                // Jeœli jest pionek, wie¿a lub królowa, to na pewno nie jest niewystarczaj¹cy materia³
//                if (std::abs(piece) == 1 || std::abs(piece) == 5 || std::abs(piece) == 6) {
//                    return false;
//                }
//            }
//        }
//    }
//
//    // Król przeciwko królowi
//    if (pieceCount == 0) {
//        return true;
//    }
//
//    // Król i goniec przeciwko królowi
//    if (pieceCount == 1 && (hasWhiteBishop || hasBlackBishop)) {
//        return true;
//    }
//
//    // Król i skoczek przeciwko królowi
//    if (pieceCount == 1 && (hasWhiteKnight || hasBlackKnight)) {
//        return true;
//    }
//
//    // Król i dwa skoczki przeciwko samemu królowi - technicznie jest mo¿liwy mat,
//    // ale praktycznie niemo¿liwy bez b³êdu przeciwnika
//    if (pieceCount == 2 && hasWhiteKnight && hasBlackKnight &&
//        !hasWhiteBishop && !hasBlackBishop) {
//        return true;
//    }
//
//    return false;
//}
//
//int GameState::getMaterialBalance() const {
//    // Oblicz ró¿nicê materia³u (wartoœæ dodatnia oznacza przewagê bia³ych)
//    int balance = 0;
//
//    // Wartoœci figur
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
//                case 1: pieceValue = rookValue; break;   // Wie¿a
//                case 2: pieceValue = knightValue; break; // Skoczek
//                case 3: pieceValue = bishopValue; break; // Goniec
//                case 5: pieceValue = queenValue; break;  // Królowa
//                case 6: pieceValue = pawnValue; break;   // Pionek
//                    // Króle s¹ pomijane w obliczeniach materia³u
//                }
//
//                if (piece > 0) {
//                    balance += pieceValue; // Bia³a figura
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