#include "Game.h"
#include <cstdio> 
#include <iostream>
#include "identification.h"
#include <SFML/Graphics.hpp>


Piece* Game::createPiece(int pieceValue) {
    switch (std::abs(pieceValue)) {
    case 1: return new Rook();
    case 2: return new Knight();
    case 3: return new Bishop();
    case 5: return new Queen();
    case 4: return new King();
    case 6: return new Pawn();
    default: return nullptr;
    }
}

void Game::movePiece(int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    const int(*board)[8] = graph.getBoard();
    int pieceValue = board[fromRow][fromCol];
    int fromPiece = graph.getChessBoard().getPieceAt(fromRow, fromCol);
    int toPiece = graph.getChessBoard().getPieceAt(toRow, toCol);

    if ((pieceValue > 0 && isWhite) || (pieceValue < 0 && !isWhite)) {
        Piece* piece = createPiece(pieceValue);
        if (toPiece == 0 || (pieceValue > 0 && toPiece < 0) || (pieceValue < 0 && toPiece > 0)) {
            bool canMove = (fromPiece != 0 && toPiece == 0 && piece != nullptr && piece->canMove(fromRow, fromCol, toRow, toCol, board, isWhite));
            bool canCapture = (toPiece != 0 && piece != nullptr && graph.getPieceAt(toRow, toCol) != isWhite && piece->canCapture(fromRow, fromCol, toRow, toCol, board, isWhite));

            if (canMove || canCapture) {
                if (!isCheckAfterMove(fromRow, fromCol, toRow, toCol, isWhite)) {
                    graph.getChessBoard().movePiece(fromRow, fromCol, toRow, toCol);
                    
                    system("cls || clear");
                }
                else {
                    printf("Król bêdzie szachowany po tym ruchu!\n");
                }
            }
            else {
                printf("Nieprawid³owy ruch pionka!\n");
            }
        }
        else {
            printf("Pole docelowe jest zajête przez w³asnego pionka!\n");
        }
        delete piece;
    }
    else {
        printf("Nie mo¿esz ruszyæ pionkiem przeciwnika!\n");
    }
}


bool Game::isCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol, bool isWhiteTurn) {
    const int(*board)[8] = graph.getBoard();
    int pieceValue = board[fromRow][fromCol];

    // Tworzenie kopii planszy po wykonaniu ruchu
    int tempBoard[8][8];
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            tempBoard[i][j] = board[i][j];
        }
    }

    // Wykonanie ruchu na kopii planszy
    int movedPiece = tempBoard[fromRow][fromCol];
    tempBoard[toRow][toCol] = movedPiece;
    tempBoard[fromRow][fromCol] = 0;

    // Aktualizacja pozycji króla na kopii planszy
    int& kingRow = isWhiteTurn ? whiteKingRow : blackKingRow;
    int& kingCol = isWhiteTurn ? whiteKingCol : blackKingCol;

    if (pieceValue == 4 || pieceValue == -4) {
        kingRow = toRow;
        kingCol = toCol;
    }

    // Sprawdzenie, czy jakikolwiek pionek przeciwnika mo¿e zaatakowaæ króla po wykonaniu ruchu
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((tempBoard[i][j] < 0 && isWhiteTurn) || (tempBoard[i][j] > 0 && !isWhiteTurn)) {
                Piece* piece = createPiece(std::abs(tempBoard[i][j]));
                if (piece->canCaptureKing(i, j, kingRow, kingCol, tempBoard, !isWhiteTurn)) {
                    delete piece;
                    return true; // Król bêdzie szachowany po tym ruchu
                }
                delete piece;
            }
        }
    }

    return false; // Król nie bêdzie szachowany po tym ruchu
}




bool Game::isCheckmate(bool isWhiteTurn) {
    const int(*board)[8] = graph.getBoard();

    // Szukamy króla, który jest szachowany
    std::vector<std::tuple<int, int, int>> attackers;
    std::vector<std::pair<int, int>> possibleBlock;
    std::vector<std::pair<int, int>> possibleKingMoves; // Dodany wektor dla mo¿liwych ruchów króla
    int kingValue = isWhiteTurn ? 4 : -4;
    std::pair<int, int> kingPosition = findKing(kingValue);
    int kingRow = kingPosition.first;
    int kingCol = kingPosition.second;

    // Zape³niamy wektor attackers, aby znaleŸæ pionki atakuj¹ce króla
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != 0 && ((board[i][j] > 0) != isWhiteTurn)) {
                Piece* piece = createPiece(board[i][j]);
                if (piece->canCaptureKing(i, j, kingRow, kingCol, board, isWhiteTurn)) {
                    attackers.push_back(std::make_tuple(i, j, board[i][j])); // Dodaj pozycjê atakuj¹cego pionka do wektora
                }
                delete piece;
            }
        }
    }

    // Zmienna logiczna przechowuj¹ca informacjê o mo¿liwoœci wyjœcia z szacha
    bool canMoveOutOfCheck = false;

    // Sprawdzamy, czy król mo¿e siê ruszyæ, aby wyjœæ ze szacha
    if (canKingMove(kingRow, kingCol, isWhiteTurn, possibleKingMoves)) {
        canMoveOutOfCheck = true;
    }

    int numberOfAttackers = attackers.size(); // Licznik pionków przeciwnika atakuj¹cych króla
    int attackerRow = -1, attackerCol = -1; // Wspó³rzêdne pionka, który szachuje króla

    // Zmienna logiczna przechowuj¹ca informacjê o mo¿liwoœci zablokowania atakuj¹cego pionka
    bool canBlockAttacker = false;

    // Sprawdzamy, czy król jest atakowany przez pojedynczego pionka i czy mo¿na go zablokowaæ
    if (numberOfAttackers == 1) {
        attackerRow = std::get<0>(attackers[0]);
        attackerCol = std::get<1>(attackers[0]);
        int attackerValue = std::abs(std::get<2>(attackers[0])); // U¿ycie wartoœci absolutnej dla attackerValue

        // Sprawdzamy, czy mo¿na zablokowaæ pionka szachuj¹cego króla
        if (canBlockCheck(kingRow, kingCol, attackerRow, attackerCol, attackerValue, isWhiteTurn, attackers, possibleBlock)) {
            canBlockAttacker = true;
        }
    }

    // Zmienna logiczna przechowuj¹ca informacjê o mo¿liwoœci zbicia atakuj¹cego pionka
    bool canCaptureAttackerPiece = false;

    // Sprawdzamy, czy król mo¿e zbiæ pionka szachuj¹cego
    if (canCaptureAttacker(kingRow, kingCol, board, attackers, isWhiteTurn)) {
        canCaptureAttackerPiece = true;
    }

    // Wyœwietlanie komunikatów tylko wtedy, gdy odpowiednia flaga zmienia wartoœæ na true
    if (canMoveOutOfCheck) {
        printf("Król mo¿e siê ruszyæ i wyjœæ ze szacha.\n");
    }

    if (canBlockAttacker) {
        printf("Mo¿na zablokowaæ ruch atakuj¹cego pionka.\n");
    }

    if (canCaptureAttackerPiece) {
        printf("Mo¿na zbiæ atakuj¹cego pionka.\n");
    }

    // Król jest szachowany i nie mo¿e siê ruszyæ, szacha nie da siê zablokowaæ ani zbiæ
    if (!canMoveOutOfCheck && !canBlockAttacker && !canCaptureAttackerPiece) {
        printf("Szach mat!\n");
        return true;
    }


    return false;
}


    

bool Game::isCheck(bool isWhiteTurn, std::vector<std::tuple<int, int, int>>& attackers) {
    const int(*board)[8] = graph.getBoard();

    // Znalezienie pozycji króla
    int kingValue = isWhiteTurn ? 4 : -4; // wartoœæ króla
    std::pair<int, int> kingPosition = findKing(kingValue);
    int kingRow = kingPosition.first;
    int kingCol = kingPosition.second;

    // Sprawdzenie, czy król jest atakowany przez jakikolwiek pionek przeciwnika
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != 0 && ((board[i][j] > 0) != isWhiteTurn)) {
                Piece* piece = createPiece(board[i][j]);
                if (piece->canCaptureKing(i, j, kingRow, kingCol, board, isWhiteTurn)) {
                    attackers.push_back(std::make_tuple(i, j, board[i][j])); // Dodaj pozycjê atakuj¹cego pionka do wektora
                    printf("Atakuj¹cy pionek na pozycji (%d, %d) z wartoœci¹ %d\n", i, j, board[i][j]);

                }
                delete piece;
            }
        }
    }

    return !attackers.empty(); // Zwróæ true jeœli jest szach, false w przeciwnym razie
}



void Game::GameLoop() {
    // Wspó³rzêdne klikniêtego pionka
    int selectedPieceRow = -1;
    int selectedPieceCol = -1;
    const int(*board)[8] = graph.getBoard();
    bool checkMessageDisplayed = false; // Flaga, która œledzi czy komunikat o szachu zosta³ ju¿ wyœwietlony
    bool moveMade = false; // Flaga, która œledzi czy wykonano ruch

    findKings(board);

    // Poka¿ planszê i okno przed rozpoczêciem pêtli
    graph.render();

    while (graph.getWindow().isOpen()) {
        sf::Event event;
        if (graph.getWindow().waitEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(graph.getWindow());
                int i = mousePosition.y / (graph.getWindow().getSize().y / 8);
                int j = mousePosition.x / (graph.getWindow().getSize().x / 8);

                bool isWhite = isWhiteTurn(); // Sprawdzenie koloru gracza

                if (selectedPieceRow == -1 && selectedPieceCol == -1) {
                    selectedPieceRow = i;
                    selectedPieceCol = j;

                    // SprawdŸ, czy jest szach
                    if (isCheck(isWhite, attackers)) {
                        if (isCheckmate(isWhite)) {
                            printf("Szach mat! Gra zakoñczona.\n");
                            // graph.getWindow().close();
                        }
                        else if (!checkMessageDisplayed) {
                            printf("Twój król jest szachowany! Musisz obroniæ króla!\n");
                            checkMessageDisplayed = true;
                        }
                    }
                    else {
                        checkMessageDisplayed = false; // Resetowanie flagi, gdy nie ma szacha
                    }

                    // SprawdŸ, czy jest mo¿liwe wykonanie ruchu
                    if ((isWhite && board[i][j] == 4) || (!isWhite && board[i][j] == -4)) {
                        if (std::find(possibleKingMoves.begin(), possibleKingMoves.end(), std::make_pair(i, j)) != possibleKingMoves.end()) {
                            printf("Wybrane pole (%d, %d) jest w wektorze possibleKingMoves.\n", i, j);
                            movePiece(selectedPieceRow, selectedPieceCol, i, j, isWhite);
                            // Aktualizacja pozycji króla po wykonaniu ruchu
                            selectedPieceRow = -1;
                            selectedPieceCol = -1;
                            moveMade = true; // Ustawienie flagi na true po wykonaniu ruchu
                        }
                    }

                }
                else {
                    movePiece(selectedPieceRow, selectedPieceCol, i, j, isWhite);
                    if (board[i][j] == 4 || board[i][j] == -4) { // Sprawdzenie, czy poruszamy królem
                        if (isWhite) {
                            whiteKingRow = i;
                            whiteKingCol = j;
                        }
                        else {
                            blackKingRow = i;
                            blackKingCol = j;
                        }
                    }
                    selectedPieceRow = -1;
                    selectedPieceCol = -1;
                    moveMade = true; // Ustawienie flagi na true po wykonaniu ruchu
                }

                attackers.clear();

                // Aktualizacja pozycji króla, jeœli wykonano ruch
                if (moveMade) {
                    printf("Bia³y król znajduje siê na pozycji (%d, %d).\n", whiteKingRow, whiteKingCol);
                    printf("Czarny król znajduje siê na pozycji (%d, %d).\n", blackKingRow, blackKingCol);
                    toggleTurn();
                    graph.render();
                    moveMade = false; // Resetowanie flagi po renderowaniu
                }
            }
            else if (event.type == sf::Event::Closed) {
                graph.getWindow().close();
            }
        }
    }
}


bool Game::isWhiteTurn() const {
    return whiteTurn;
}

// Metoda do zmiany gracza (bia³y -> czarny, czarny -> bia³y)
void Game::toggleTurn() {
    whiteTurn = !whiteTurn;
}


std::pair<int, int> Game::findKing(int kingValue) {
    const int(*board)[8] = graph.getBoard();
    std::pair<int, int> kingPosition = { -1, -1 };

    // Znalezienie pozycji króla
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] == kingValue) {
                kingPosition.first = i;
                kingPosition.second = j;
                break;
            }
        }
        if (kingPosition.first != -1) break;
    }

    return kingPosition;
}


void Game::findKings(const int(*board)[8]) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] == 4) { // Bia³y król
                whiteKingRow = i;
                whiteKingCol = j;
            }
            else if (board[i][j] == -4) { // Czarny król
                blackKingRow = i;
                blackKingCol = j;
            }
        }
    }
}





bool Game::canKingMove(int kingRow, int kingCol, bool isWhiteTurn, std::vector<std::pair<int, int>>& possibleKingMoves) {
    const int(*board)[8] = graph.getBoard();

    // Wspó³rzêdne, na które król mo¿e siê ruszyæ
    int moveOffsets[3] = { -1, 0, 1 };

    // Wektor przechowuj¹cy poprawne ruchy króla
    std::vector<std::pair<int, int>> validMoves;

    for (int i : moveOffsets) {
        for (int j : moveOffsets) {
            if (i == 0 && j == 0) continue; // Pomijamy pozycjê króla

            int newRow = kingRow + i;
            int newCol = kingCol + j;

            // Sprawdzamy, czy nowa pozycja mieœci siê na planszy i jest pusta lub zajêta przez przeciwnika
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8 &&
                ((board[newRow][newCol] == 0) ||
                    (isWhiteTurn && board[newRow][newCol] < 0) ||
                    (!isWhiteTurn && board[newRow][newCol] > 0))) {
                validMoves.push_back({ newRow, newCol });
            }
        }
    }

    // Dodajemy mo¿liwe ruchy króla do wektora possibleKingMoves
    for (const auto& move : validMoves) {
        int newRow = move.first;
        int newCol = move.second;

        // Sprawdzamy, czy król bêdzie szachowany po wykonaniu ruchu
        if (!isCheckAfterMove(kingRow, kingCol, newRow, newCol, isWhiteTurn)) {
            possibleKingMoves.push_back({ newRow, newCol }); // Dodajemy mo¿liwy ruch króla
        }
    }

    // Jeœli król ma mo¿liwe ruchy, zwracamy true, w przeciwnym razie false
    bool canMove = !possibleKingMoves.empty();

    // Wypisanie zawartoœci wektora possibleKingMoves
    printf("Mo¿liwe ruchy króla:\n");
    for (const auto& move : possibleKingMoves) {
        printf("Pozycja: (%d, %d)\n", move.first, move.second);
    }

    return canMove;
}


bool Game::canCaptureAttacker(int toRow, int toCol, const int board[8][8], const std::vector<std::tuple<int, int, int>>& attackers, bool isWhiteTurn) {
    // Iteracja po planszy
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // Sprawdzenie czy pionek na danej pozycji nale¿y do gracza, który ma teraz ruch
            if ((isWhiteTurn && board[i][j] > 0) || (!isWhiteTurn && board[i][j] < 0)) {
                Piece* piece = createPiece(board[i][j]); // Tworzenie instancji odpowiedniego pionka

                int const pieceValue = board[i][j];

                // Iteracja po atakuj¹cych pionkach
                for (const auto& attacker : attackers) {
                    int fromRow = std::get<0>(attacker);
                    int fromCol = std::get<1>(attacker);
                    int attackerValue = std::get<2>(attacker);

                  //  std::cout << "Atakuj¹cy: " << fromRow << ", " << fromCol << ", " << attackerValue << std::endl;


                    // Sprawdzenie czy pionek na danej pozycji mo¿e zbic atakuj¹cego pionka
                    if (piece->canCapturePiece(i, j, fromRow, fromCol, board, pieceValue, isWhiteTurn)) {
                        // Wyœwietlenie komunikatu o pionku, który mo¿e zbic atakuj¹cego pionka
                        printf("Pionek na pozycji (%d, %d) mo¿e zbiæ atakuj¹cego pionka.\n", i, j);
                        delete piece;
                        return true;
                    }
                }
                delete piece;
            }
        }
    }
    return false;
}




bool Game::canBlockCheck(int kingRow, int kingCol, int attackerRow, int attackerCol, int attackerValue, bool isWhiteTurn, const std::vector<std::tuple<int, int, int>>& attackers, std::vector<std::pair<int, int>>& possibleBlock) {
    const int(*board)[8] = graph.getBoard();

    // Iteracja po atakuj¹cych pionkach
    for (const auto& attacker : attackers) {
        int value = std::abs(std::get<2>(attacker)); // U¿ycie wartoœci absolutnej

        // Jeœli atakuj¹cy pionek to kon lub pionek, nie da siê go zablokowaæ
        if (value == 2 || value == 6)
            continue;

        // Jeœli atakuj¹cy pionek to wie¿a
        if (value == 1) {
            int deltaX = kingCol - attackerCol;
            int deltaY = kingRow - attackerRow;

            // Sprawdzenie, czy wie¿a szachuje króla w poziomie lub pionie
            if (deltaX == 0 || deltaY == 0) {
                // Dodawanie pustych pól miêdzy królem a wie¿¹ w pionie
                if (deltaX == 0) {
                    int stepY = (deltaY > 0) ? 1 : -1;
                    for (int y = kingRow + stepY; y != attackerRow; y += stepY) {
                        if (board[y][kingCol] == 0) {
                            possibleBlock.push_back({ y, kingCol });
                        }
                    }
                }
                // Dodawanie pustych pól miêdzy królem a wie¿¹ w poziomie
                else if (deltaY == 0) {
                    int stepX = (deltaX > 0) ? 1 : -1;
                    for (int x = kingCol + stepX; x != attackerCol; x += stepX) {
                        if (board[kingRow][x] == 0) {
                            possibleBlock.push_back({ kingRow, x });
                        }
                    }
                }
            }
        }

        // Jeœli atakuj¹cym pionkiem jest goniec
        if (value == 3) {
            int deltaX = kingCol - attackerCol;
            int deltaY = kingRow - attackerRow;

            // Sprawdzenie, czy goniec szachuje króla po skosie
            if (std::abs(deltaX) == std::abs(deltaY)) {
                int stepX = (deltaX > 0) ? 1 : -1;
                int stepY = (deltaY > 0) ? 1 : -1;
                int x = attackerCol + stepX;
                int y = attackerRow + stepY;

                while (x != kingCol && y != kingRow) {
                    if (board[y][x] == 0) {
                        possibleBlock.push_back({ y, x });
                    }
                    x += stepX;
                    y += stepY;
                }
            }
        }

        // Jeœli atakuj¹cym pionkiem jest królowa
        if (value == 5) {
            int deltaX = kingCol - attackerCol;
            int deltaY = kingRow - attackerRow;

            // Sprawdzenie, czy królowa szachuje króla w poziomie, pionie lub po skosie
            if (deltaX == 0 || deltaY == 0 || std::abs(deltaX) == std::abs(deltaY)) {
                // Dodawanie pustych pól miêdzy królem a królow¹ w pionie
                if (deltaX == 0) {
                    int stepY = (deltaY > 0) ? 1 : -1;
                    for (int y = kingRow + stepY; y != attackerRow; y += stepY) {
                        if (board[y][kingCol] == 0) {
                            possibleBlock.push_back({ y, kingCol });
                        }
                    }
                }
                // Dodawanie pustych pól miêdzy królem a królow¹ w poziomie
                else if (deltaY == 0) {
                    int stepX = (deltaX > 0) ? 1 : -1;
                    for (int x = kingCol + stepX; x != attackerCol; x += stepX) {
                        if (board[kingRow][x] == 0) {
                            possibleBlock.push_back({ kingRow, x });
                        }
                    }
                }
                // Dodawanie pustych pól miêdzy królem a królow¹ po skosie
                else if (std::abs(deltaX) == std::abs(deltaY)) {
                    int stepX = (deltaX > 0) ? 1 : -1;
                    int stepY = (deltaY > 0) ? 1 : -1;
                    int x = attackerCol + stepX;
                    int y = attackerRow + stepY;

                    while (x != kingCol && y != kingRow) {
                        if (board[y][x] == 0) {
                            possibleBlock.push_back({ y, x });
                        }
                        x += stepX;
                        y += stepY;
                    }
                }
            }
        }
    }

    // Wyœwietlenie zawartoœci wektora possibleBlock
    printf("Possible block positions: ");
    for (const auto& pos : possibleBlock) {
        printf("(%d, %d) ", pos.first, pos.second);
    }
    printf("\n");


    return !possibleBlock.empty(); // Zwraca true, jeœli mo¿liwe s¹ ruchy blokuj¹ce
}


/*
void Game::checkPromotion() {
    const int(*board)[8] = graph.getBoard();
    const int promotionRowWhite = 0; // Rz¹d, na którym nastêpuje promocja dla bia³ych
    const int promotionRowBlack = 7; // Rz¹d, na którym nastêpuje promocja dla czarnych

    // SprawdŸ dla wszystkich pionków na odpowiednich rzêdach planszy
    // dla bia³ych pionków sprawdzamy rz¹d 0
    for (int col = 0; col < 8; ++col) {
        if (board[promotionRowWhite][col] == 6) {
            // Promuj pionka dla bia³ych
            graph.promotePawn(graph.getWindow(), promotionRowWhite, col, true);
        }
    }
    // dla czarnych pionków sprawdzamy rz¹d 7
    for (int col = 0; col < 8; ++col) {
        if (board[promotionRowBlack][col] == -6) {
            // Promuj pionka dla czarnych
            graph.promotePawn(graph.getWindow(), promotionRowBlack, col, false);
        }
    }
}

*/
