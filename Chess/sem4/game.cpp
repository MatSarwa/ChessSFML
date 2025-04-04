//#include "Game.h"
//#include "Piece.h"
//#include "Pawn.h"
//#include "Rook.h"
//#include "Knight.h"
//#include "Bishop.h"
//#include "Queen.h"
//#include "King.h"
//#include <iostream>
//#include <algorithm>
//#include <memory>
//
//Game::Game()
//    : whiteTurn(true),
//    selectedPieceRow(-1),
//    selectedPieceCol(-1),
//    pieceSelected(false),  // Zmieniona nazwa zmiennej
//    lastMoveFromRow(-1),
//    lastMoveFromCol(-1),
//    lastMoveToRow(-1),
//    lastMoveToCol(-1),
//    lastCapturedPiece(0),
//    lastMoveWasCapture(false)
//{
//    init();
//}
//
//Game::~Game() {
//    // Nic do zwalniania - u�ywamy unique_ptr, kt�re zajm� si� usuni�ciem obiekt�w
//}
//
//void Game::init() {
//    // Inicjalizacja komponent�w
//    validator = std::make_unique<MoveValidator>(board);
//    notation = std::make_unique<ChessNotation>(board);
//    gameState = std::make_unique<GameState>(board, *validator);
//    moveHistory = std::make_unique<MoveHistory>();
//
//    // Resetowanie stanu gry
//    reset();
//}
//
//void Game::reset() {
//    // Resetowanie planszy
//    board.reset();
//
//    // Resetowanie stanu gry
//    whiteTurn = true;
//    selectedPieceRow = -1;
//    selectedPieceCol = -1;
//    pieceSelected = false;  // Zmieniona nazwa zmiennej
//
//    // Resetowanie zmiennych dla ostatniego ruchu
//    lastMoveFromRow = -1;
//    lastMoveFromCol = -1;
//    lastMoveToRow = -1;
//    lastMoveToCol = -1;
//    lastCapturedPiece = 0;
//    lastMoveWasCapture = false;
//    lastMoveNotation = "";
//
//    // Czyszczenie bufor�w
//    possibleMoves.clear();
//    attackingPieces.clear();
//
//    // Resetowanie komponent�w
//    if (validator) validator->updateKingsPositions();
//    if (gameState) gameState->reset();
//    if (moveHistory) moveHistory->clear();
//}
//
//void Game::update() {
//    // Aktualizacja stanu gry
//    if (gameState) {
//        gameState->update();
//    }
//
//    // Aktualizacja walidatora
//    if (validator) {
//        validator->updateKingsPositions();
//    }
//}
//
//void Game::handleInput(const sf::Event& event, const sf::Vector2i& mousePos) {
//    // Obs�uga klikni�� mysz�
//    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
//        // Przelicz pozycj� myszy na wsp�rz�dne planszy
//        int boardSize = 600; // Za��my, �e plansza ma rozmiar 600x600 pikseli
//        int squareSize = boardSize / 8;
//
//        int col = mousePos.x / squareSize;
//        int row = mousePos.y / squareSize;
//
//        // Sprawd� czy klikni�cie jest w granicach planszy
//        if (row >= 0 && row < 8 && col >= 0 && col < 8) {
//            if (!pieceSelected) {  // Zmieniona nazwa zmiennej
//                // Wyb�r figury
//                int piece = board.getPieceAt(row, col);
//                bool isWhitePiece = (piece > 0);
//
//                // Sprawd� czy wybrano figur� w�a�ciwego koloru
//                if (piece != 0 && isWhitePiece == whiteTurn) {
//                    selectedPieceRow = row;
//                    selectedPieceCol = col;
//                    pieceSelected = true;  // Zmieniona nazwa zmiennej
//
//                    // Pobierz mo�liwe ruchy dla wybranej figury
//                    possibleMoves = getPossibleMoves(row, col);
//                }
//            }
//            else {
//                // Wykonanie ruchu lub wyb�r innej figury
//                int piece = board.getPieceAt(row, col);
//                bool isWhitePiece = (piece > 0);
//
//                // Je�li klikni�to figur� tego samego koloru, zmie� wyb�r
//                if (piece != 0 && isWhitePiece == whiteTurn) {
//                    selectedPieceRow = row;
//                    selectedPieceCol = col;
//
//                    // Pobierz mo�liwe ruchy dla nowo wybranej figury
//                    possibleMoves = getPossibleMoves(row, col);
//                }
//                else {
//                    // Pr�ba wykonania ruchu
//                    if (makeMove(selectedPieceRow, selectedPieceCol, row, col)) {
//                        // Ruch wykonany, zresetuj wyb�r
//                        pieceSelected = false;  // Zmieniona nazwa zmiennej
//                        possibleMoves.clear();
//                    }
//                }
//            }
//        }
//    }
//    else if (event.type == sf::Event::KeyPressed) {
//        // Obs�uga klawiatury
//        if (event.key.code == sf::Keyboard::Escape) {
//            // Anulowanie wyboru figury
//            pieceSelected = false;  // Zmieniona nazwa zmiennej
//            possibleMoves.clear();
//        }
//        else if (event.key.code == sf::Keyboard::Z && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
//            // Cofni�cie ruchu (CTRL+Z)
//            undoLastMove();
//        }
//    }
//}
//
//bool Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
//    // Sprawd� czy ruch jest mo�liwy
//    if (!validator || !validator->isValidMove(fromRow, fromCol, toRow, toCol, whiteTurn)) {
//        return false;
//    }
//
//    // Zapisz informacje o ruchu przed jego wykonaniem
//    lastMoveFromRow = fromRow;
//    lastMoveFromCol = fromCol;
//    lastMoveToRow = toRow;
//    lastMoveToCol = toCol;
//    lastCapturedPiece = board.getPieceAt(toRow, toCol);
//    lastMoveWasCapture = (lastCapturedPiece != 0);
//
//    // Wykonaj ruch na planszy
//    board.movePiece(fromRow, fromCol, toRow, toCol);
//
//    // Sprawd� czy pionek wymaga promocji
//    int piece = board.getPieceAt(toRow, toCol);
//    if ((piece == 6 && toRow == 0) || (piece == -6 && toRow == 7)) {
//        // Automatyczna promocja do kr�lowej (w interfejsie u�ytkownik b�dzie m�g� wybra� figur�)
//        board.promotePawn(toRow, toCol, whiteTurn ? 5 : -5);
//    }
//
//    // Zapisz notacj� ruchu
//    if (notation) {
//        ChessMove move;
//        move.fromRow = fromRow;
//        move.fromCol = fromCol;
//        move.toRow = toRow;
//        move.toCol = toCol;
//        move.movedPiece = piece;
//        move.capturedPiece = lastCapturedPiece;
//        move.isCheck = isCheck(!whiteTurn);
//        move.isCheckmate = isCheckmate(!whiteTurn);
//        move.isPromotion = false; // Tutaj musia�aby by� logika promocji
//        move.promotionPiece = 0;
//
//        lastMoveNotation = notation->moveToAlgebraic(move);
//
//        // Dodaj ruch do historii
//        if (moveHistory) {
//            moveHistory->addMove(move);
//        }
//    }
//
//    // Zmie� tur�
//    whiteTurn = !whiteTurn;
//
//    // Aktualizuj stan gry
//    update();
//
//    return true;
//}
//
//bool Game::undoLastMove() {
//    if (!moveHistory || moveHistory->isEmpty()) {
//        return false;
//    }
//
//    // Pobierz ostatni ruch
//    ChessMove lastMove = moveHistory->getLastMove();
//    moveHistory->removeLastMove();
//
//    // Cofnij ruch na planszy
//    board.undoLastMove();
//
//    // Zmie� tur� z powrotem
//    whiteTurn = !whiteTurn;
//
//    // Aktualizuj stan gry
//    update();
//
//    return true;
//}
//
//std::vector<sf::Vector2i> Game::getPossibleMoves(int row, int col) {
//    std::vector<sf::Vector2i> moves;
//
//    if (validator) {
//        // U�yj walidatora do uzyskania mo�liwych ruch�w
//        moves = validator->getPossibleMoves(row, col, whiteTurn);
//    }
//
//    return moves;
//}
//
//bool Game::isGameOver() const {
//    if (gameState) {
//        return gameState->isGameOver();
//    }
//
//    // Je�li nie ma gameState, sprawd� prostsze warunki
//    return isCheckmate(whiteTurn) || isStalemate(whiteTurn);
//}
//
//bool Game::isCheck(bool forWhite) const {
//    if (validator) {
//        return validator->isKingInCheck(forWhite);
//    }
//    return false;
//}
//
//bool Game::isCheckmate(bool forWhite) const {
//    if (validator) {
//        return validator->isCheckmate(forWhite);
//    }
//    return false;
//}
//
//bool Game::isStalemate(bool forWhite) const {
//    if (validator) {
//        return validator->isStalemate(forWhite);
//    }
//    return false;
//}
//
//GameResult Game::getGameResult() const {
//    if (gameState) {
//        return gameState->getResult();
//    }
//
//    // Prostsze sprawdzenie je�li nie ma gameState
//    if (isCheckmate(true)) {
//        return GameResult::BLACK_WIN;
//    }
//    else if (isCheckmate(false)) {
//        return GameResult::WHITE_WIN;
//    }
//    else if (isStalemate(true) || isStalemate(false)) {
//        return GameResult::DRAW;
//    }
//
//    return GameResult::ONGOING;
//}
//
//std::string Game::getResultReason() const {
//    if (gameState) {
//        return gameState->getResultReason();
//    }
//
//    // Prostsze sprawdzenie je�li nie ma gameState
//    if (isCheckmate(true)) {
//        return "Czarny wygrywa (szach-mat)";
//    }
//    else if (isCheckmate(false)) {
//        return "Bia�y wygrywa (szach-mat)";
//    }
//    else if (isStalemate(true) || isStalemate(false)) {
//        return "Remis (pat)";
//    }
//
//    return "";
//}
//
//bool Game::needsPromotion() const {
//    // Sprawd� czy na planszy jest pionek, kt�ry wymaga promocji
//    for (int col = 0; col < 8; col++) {
//        // Bia�y pionek na ostatnim rz�dzie
//        if (board.getPieceAt(0, col) == 6) {
//            return true;
//        }
//        // Czarny pionek na pierwszym rz�dzie
//        if (board.getPieceAt(7, col) == -6) {
//            return true;
//        }
//    }
//    return false;
//}
//
//bool Game::promotePawn(int row, int col, int newPiece) {
//    // Sprawd� czy promocja jest mo�liwa i wykonaj j�
//    if (board.canPromotePawn(row, col)) {
//        bool success = board.promotePawn(row, col, newPiece);
//        update();
//        return success;
//    }
//    return false;
//}
//
//// Getter dla isPieceSelected zmieniony na korzystanie z pola pieceSelected
//bool Game::isPieceSelected() const {
//    return pieceSelected;
//}