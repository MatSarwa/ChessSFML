#include "Game.h"
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

void Game::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    const int(*board)[8] = graph.getBoard();
    int pieceValue = board[fromRow][fromCol];
    int fromPiece = graph.getChessBoard().getPieceAt(fromRow, fromCol);
    int toPiece = graph.getChessBoard().getPieceAt(toRow, toCol);

    

    if ((pieceValue > 0 && isWhiteTurn()) || (pieceValue < 0 && !isWhiteTurn())) {
        Piece* piece = createPiece(pieceValue);
        if (toPiece == 0 || (pieceValue > 0 && toPiece < 0) || (pieceValue < 0 && toPiece > 0)) {
            // Tutaj sprawdzamy, czy pionek mo�e si� poruszy� na dane pole lub czy mo�e zaatakowa� pionka przeciwnika na tym polu
            if ((fromPiece != 0 && toPiece == 0 && piece != nullptr && piece->canMove(fromRow, fromCol, toRow, toCol, board, isWhiteTurn())) ||
                (toPiece != 0 && piece != nullptr && graph.getPieceAt(toRow, toCol) != isWhiteTurn() && piece->canCapture(fromRow, fromCol, toRow, toCol, board, isWhiteTurn()))) {

                if (!isCheckAfterMove(fromRow, fromCol, toRow, toCol, isWhiteTurn())) {
                    // Aktualizacja planszy
                    graph.getChessBoard().movePiece(fromRow, fromCol, toRow, toCol);

                    // Zmiana gracza po wykonaniu ruchu
                    toggleTurn();

                    // Wymazanie konsoli
                    system("cls || clear");
                }
                else {
                    std::cout << "Kr�l b�dzie szachowany po tym ruchu!" << std::endl;
                }
            }
            delete piece;
        }
        else {
            std::cout << "Pole docelowe jest zaj�te przez w�asnego pionka!" << std::endl;
        }
    }
    else {
        std::cout << "Nie mo�esz ruszy� pionkiem przeciwnika!" << std::endl;
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

    // Znalezienie pozycji kr�la
    int kingValue = isWhiteTurn ? 4 : -4; // warto�� kr�la
    int kingRow = -1, kingCol = -1;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (tempBoard[i][j] == kingValue) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (kingRow != -1) break;
    }

    // Sprawdzenie, czy jakikolwiek pionek przeciwnika mo�e zaatakowa� kr�la po wykonaniu ruchu
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((tempBoard[i][j] < 0 && isWhiteTurn) || (tempBoard[i][j] > 0 && !isWhiteTurn)) {
                Piece* piece = createPiece(std::abs(tempBoard[i][j]));
                if (piece->canCaptureKing(i, j, kingRow, kingCol, tempBoard, !isWhiteTurn)) {
                    delete piece;
                    return true; // Kr�l b�dzie szachowany po tym ruchu
                }
                delete piece;
            }
        }
    }

    return false; // Kr�l nie b�dzie szachowany po tym ruchu
}



bool Game::isCheckmate(bool isWhiteTurn) {
    const int(*board)[8] = graph.getBoard();

    // Szukamy kr�la, kt�ry jest szachowany
    std::vector<std::tuple<int, int, int>> attackers;
    std::vector<std::pair<int, int>> possibleBlock;
    std::vector<std::pair<int, int>> possibleKingMoves; // Dodany wektor dla mo�liwych ruch�w kr�la
    int kingValue = isWhiteTurn ? 4 : -4;
    std::pair<int, int> kingPosition = findKing(kingValue);
    int kingRow = kingPosition.first;
    int kingCol = kingPosition.second;

    // Zape�niamy wektor attackers, aby znale�� pionki atakuj�ce kr�la
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != 0 && ((board[i][j] > 0) != isWhiteTurn)) {
                Piece* piece = createPiece(board[i][j]);
                if (piece->canCaptureKing(i, j, kingRow, kingCol, board, isWhiteTurn)) {
                    attackers.push_back(std::make_tuple(i, j, board[i][j])); // Dodaj pozycj� atakuj�cego pionka do wektora
                }
                delete piece;
            }
        }
    }

    // Zmienna logiczna przechowuj�ca informacj� o mo�liwo�ci wyj�cia z szacha
    bool canMoveOutOfCheck = false;

    // Sprawdzamy, czy kr�l mo�e si� ruszy�, aby wyj�� ze szacha
    if (canKingMove(kingRow, kingCol, isWhiteTurn, possibleKingMoves)) {
        canMoveOutOfCheck = true;
    }

    int numberOfAttackers = attackers.size(); // Licznik pionk�w przeciwnika atakuj�cych kr�la
    int attackerRow = -1, attackerCol = -1; // Wsp�rz�dne pionka, kt�ry szachuje kr�la

    // Zmienna logiczna przechowuj�ca informacj� o mo�liwo�ci zablokowania atakuj�cego pionka
    bool canBlockAttacker = false;

    // Sprawdzamy, czy kr�l jest atakowany przez pojedynczego pionka i czy mo�na go zablokowa�
    if (numberOfAttackers == 1) {
        attackerRow = std::get<0>(attackers[0]);
        attackerCol = std::get<1>(attackers[0]);
        int attackerValue = std::abs(std::get<2>(attackers[0])); // U�ycie warto�ci absolutnej dla attackerValue

        // Sprawdzamy, czy mo�na zablokowa� pionka szachuj�cego kr�la
        if (canBlockCheck(kingRow, kingCol, attackerRow, attackerCol, attackerValue, isWhiteTurn, attackers, possibleBlock)) {
            canBlockAttacker = true;
        }
    }

    // Zmienna logiczna przechowuj�ca informacj� o mo�liwo�ci zbicia atakuj�cego pionka
    bool canCaptureAttackerPiece = false;

    // Sprawdzamy, czy kr�l mo�e zbi� pionka szachuj�cego
    if (canCaptureAttacker(kingRow, kingCol, board, attackers, isWhiteTurn)) {
        canCaptureAttackerPiece = true;
    }

    // Wy�wietlanie komunikat�w tylko wtedy, gdy odpowiednia flaga zmienia warto�� na true
    if (canMoveOutOfCheck) {
        std::cout << "Kr�l mo�e si� ruszy� i wyj�� ze szacha." << std::endl;
    }

    if (canBlockAttacker) {
        std::cout << "Mo�na zablokowa� ruch atakuj�cego pionka." << std::endl;
    }

    if (canCaptureAttackerPiece) {
        std::cout << "Mo�na zbi� atakuj�cego pionka." << std::endl;
    }

    // Kr�l jest szachowany i nie mo�e si� ruszy�, szacha nie da si� zablokowa� ani zbi�
    if (!canMoveOutOfCheck && !canBlockAttacker && !canCaptureAttackerPiece) {
        std::cout << "Szach mat!" << std::endl;
        return true;
    }

    return false;
}


    

bool Game::isCheck(bool isWhiteTurn, std::vector<std::tuple<int, int, int>>& attackers) {
    const int(*board)[8] = graph.getBoard();

    // Znalezienie pozycji kr�la
    int kingValue = isWhiteTurn ? 4 : -4; // warto�� kr�la
    std::pair<int, int> kingPosition = findKing(kingValue);
    int kingRow = kingPosition.first;
    int kingCol = kingPosition.second;

    // Sprawdzenie, czy kr�l jest atakowany przez jakikolwiek pionek przeciwnika
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != 0 && ((board[i][j] > 0) != isWhiteTurn)) {
                Piece* piece = createPiece(board[i][j]);
                if (piece->canCaptureKing(i, j, kingRow, kingCol, board, isWhiteTurn)) {
                    attackers.push_back(std::make_tuple(i, j, board[i][j])); // Dodaj pozycj� atakuj�cego pionka do wektora
                    std::cout << "Atakuj�cy pionek na pozycji (" << i << ", " << j << ") z warto�ci� " << board[i][j] << std::endl;
                }
                delete piece;
            }
        }
    }

    return !attackers.empty(); // Zwr�� true je�li jest szach, false w przeciwnym razie
}



void Game::GameLoop() {
    // Wsp�rz�dne klikni�tego pionka
    int selectedPieceRow = -1;
    int selectedPieceCol = -1;
    const int(*board)[8] = graph.getBoard();
    bool checkMessageDisplayed = false; // Flaga, kt�ra �ledzi czy komunikat o szachu zosta� ju� wy�wietlony
    bool moveMade = false; // Flaga, kt�ra �ledzi czy wykonano ruch

    // Poka� plansz� i okno przed rozpocz�ciem p�tli
    graph.render();
    while (graph.getWindow().isOpen()) {
        sf::Event event;
        while (graph.getWindow().pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(graph.getWindow());
                    int i = mousePosition.y / (graph.getWindow().getSize().y / 8);
                    int j = mousePosition.x / (graph.getWindow().getSize().x / 8);

                    if (selectedPieceRow == -1 && selectedPieceCol == -1) {
                        selectedPieceRow = i;
                        selectedPieceCol = j;

                        // Sprawd�, czy jest szach
                        if (isCheck(isWhiteTurn(), attackers)) {
                            if (isCheckmate(isWhiteTurn())) {
                                std::cout << "Szach mat! Gra zako�czona." << std::endl;
                            //    graph.getWindow().close();
                            }
                            else if (!checkMessageDisplayed) {
                                std::cout << "Tw�j kr�l jest szachowany! Musisz obroni� kr�la!" << std::endl;
                                checkMessageDisplayed = true;
                            }
                        }
                        else {
                            checkMessageDisplayed = false; // Resetowanie flagi, gdy nie ma szacha
                        }

                        // Sprawd�, czy jest mo�liwe wykonanie ruchu
                        if ((isWhiteTurn() && board[i][j] == 4) || (!isWhiteTurn() && board[i][j] == -4)) {
                            if (std::find(possibleKingMoves.begin(), possibleKingMoves.end(), std::make_pair(i, j)) != possibleKingMoves.end()) {
                                std::cout << "Wybrane pole (" << i << ", " << j << ") jest w wektorze possibleKingMoves." << std::endl;
                                movePiece(selectedPieceRow, selectedPieceCol, i, j);
                                selectedPieceRow = -1;
                                selectedPieceCol = -1;
                                moveMade = true; // Ustawienie flagi na true po wykonaniu ruchu
                            }
                        }
                    }
                    else {
                        movePiece(selectedPieceRow, selectedPieceCol, i, j);
                        selectedPieceRow = -1;
                        selectedPieceCol = -1;
                        moveMade = true; // Ustawienie flagi na true po wykonaniu ruchu
                    }
                }
            }
            if (event.type == sf::Event::Closed) {
                graph.getWindow().close();
            }
        }

        attackers.clear();

        // Renderowanie gry tylko je�li wykonano ruch
        if (moveMade) {
            graph.render();
            moveMade = false; // Resetowanie flagi po renderowaniu
        }
    }
}







bool Game::isWhiteTurn() const {
    return whiteTurn;
}

// Metoda do zmiany gracza (bia�y -> czarny, czarny -> bia�y)
void Game::toggleTurn() {
    whiteTurn = !whiteTurn;
}


std::pair<int, int> Game::findKing(int kingValue) {
    const int(*board)[8] = graph.getBoard();
    std::pair<int, int> kingPosition = { -1, -1 };

    // Znalezienie pozycji kr�la
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


bool Game::canKingMove(int kingRow, int kingCol, bool isWhiteTurn, std::vector<std::pair<int, int>>& possibleKingMoves) {
    const int(*board)[8] = graph.getBoard();

    // Wsp�rz�dne, na kt�re kr�l mo�e si� ruszy�
    int moveOffsets[3] = { -1, 0, 1 };

    // Wektor przechowuj�cy poprawne ruchy kr�la
    std::vector<std::pair<int, int>> validMoves;

    for (int i : moveOffsets) {
        for (int j : moveOffsets) {
            if (i == 0 && j == 0) continue; // Pomijamy pozycj� kr�la

            int newRow = kingRow + i;
            int newCol = kingCol + j;

            // Sprawdzamy, czy nowa pozycja mie�ci si� na planszy i jest pusta lub zaj�ta przez przeciwnika
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8 &&
                ((board[newRow][newCol] == 0) ||
                    (isWhiteTurn && board[newRow][newCol] < 0) ||
                    (!isWhiteTurn && board[newRow][newCol] > 0))) {
                validMoves.push_back({ newRow, newCol });
            }
        }
    }

    // Dodajemy mo�liwe ruchy kr�la do wektora possibleKingMoves
    for (const auto& move : validMoves) {
        int newRow = move.first;
        int newCol = move.second;

        // Sprawdzamy, czy kr�l b�dzie szachowany po wykonaniu ruchu
        if (!isCheckAfterMove(kingRow, kingCol, newRow, newCol, isWhiteTurn)) {
            possibleKingMoves.push_back({ newRow, newCol }); // Dodajemy mo�liwy ruch kr�la
        }
    }

    // Je�li kr�l ma mo�liwe ruchy, zwracamy true, w przeciwnym razie false
    bool canMove = !possibleKingMoves.empty();

    // Wypisanie zawarto�ci wektora possibleKingMoves
    std::cout << "Mo�liwe ruchy kr�la:" << std::endl;
    for (const auto& move : possibleKingMoves) {
        std::cout << "Pozycja: (" << move.first << ", " << move.second << ")" << std::endl;
    }

    return canMove;
}


bool Game::canCaptureAttacker(int toRow, int toCol, const int board[8][8], const std::vector<std::tuple<int, int, int>>& attackers, bool isWhiteTurn) {
    // Iteracja po planszy
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // Sprawdzenie czy pionek na danej pozycji nale�y do gracza, kt�ry ma teraz ruch
            if ((isWhiteTurn && board[i][j] > 0) || (!isWhiteTurn && board[i][j] < 0)) {
                Piece* piece = createPiece(board[i][j]); // Tworzenie instancji odpowiedniego pionka

                int const pieceValue = board[i][j];

                // Iteracja po atakuj�cych pionkach
                for (const auto& attacker : attackers) {
                    int fromRow = std::get<0>(attacker);
                    int fromCol = std::get<1>(attacker);
                    int attackerValue = std::get<2>(attacker);

                  //  std::cout << "Atakuj�cy: " << fromRow << ", " << fromCol << ", " << attackerValue << std::endl;


                    // Sprawdzenie czy pionek na danej pozycji mo�e zbic atakuj�cego pionka
                    if (piece->canCapturePiece(i, j, fromRow, fromCol, board, pieceValue, isWhiteTurn)) {
                        // Wy�wietlenie komunikatu o pionku, kt�ry mo�e zbic atakuj�cego pionka
                        std::cout << "Pionek na pozycji (" << i << ", " << j << ") mo�e zbi� atakuj�cego pionka." << std::endl;
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

    // Iteracja po atakuj�cych pionkach
    for (const auto& attacker : attackers) {
        int value = std::abs(std::get<2>(attacker)); // U�ycie warto�ci absolutnej

        // Je�li atakuj�cy pionek to kon lub pionek, nie da si� go zablokowa�
        if (value == 2 || value == 6)
            continue;

        // Je�li atakuj�cy pionek to wie�a
        if (value == 1) {
            int deltaX = kingCol - attackerCol;
            int deltaY = kingRow - attackerRow;

            // Sprawdzenie, czy wie�a szachuje kr�la w poziomie lub pionie
            if (deltaX == 0 || deltaY == 0) {
                // Dodawanie pustych p�l mi�dzy kr�lem a wie�� w pionie
                if (deltaX == 0) {
                    int stepY = (deltaY > 0) ? 1 : -1;
                    for (int y = kingRow + stepY; y != attackerRow; y += stepY) {
                        if (board[y][kingCol] == 0) {
                            possibleBlock.push_back({ y, kingCol });
                        }
                    }
                }
                // Dodawanie pustych p�l mi�dzy kr�lem a wie�� w poziomie
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

        // Je�li atakuj�cym pionkiem jest goniec
        if (value == 3) {
            int deltaX = kingCol - attackerCol;
            int deltaY = kingRow - attackerRow;

            // Sprawdzenie, czy goniec szachuje kr�la po skosie
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

        // Je�li atakuj�cym pionkiem jest kr�lowa
        if (value == 5) {
            int deltaX = kingCol - attackerCol;
            int deltaY = kingRow - attackerRow;

            // Sprawdzenie, czy kr�lowa szachuje kr�la w poziomie, pionie lub po skosie
            if (deltaX == 0 || deltaY == 0 || std::abs(deltaX) == std::abs(deltaY)) {
                // Dodawanie pustych p�l mi�dzy kr�lem a kr�low� w pionie
                if (deltaX == 0) {
                    int stepY = (deltaY > 0) ? 1 : -1;
                    for (int y = kingRow + stepY; y != attackerRow; y += stepY) {
                        if (board[y][kingCol] == 0) {
                            possibleBlock.push_back({ y, kingCol });
                        }
                    }
                }
                // Dodawanie pustych p�l mi�dzy kr�lem a kr�low� w poziomie
                else if (deltaY == 0) {
                    int stepX = (deltaX > 0) ? 1 : -1;
                    for (int x = kingCol + stepX; x != attackerCol; x += stepX) {
                        if (board[kingRow][x] == 0) {
                            possibleBlock.push_back({ kingRow, x });
                        }
                    }
                }
                // Dodawanie pustych p�l mi�dzy kr�lem a kr�low� po skosie
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

    // Wy�wietlenie zawarto�ci wektora possibleBlock
    std::cout << "Possible block positions: ";
    for (const auto& pos : possibleBlock) {
        std::cout << "(" << pos.first << ", " << pos.second << ") ";
    }
    std::cout << std::endl;

    return !possibleBlock.empty(); // Zwraca true, je�li mo�liwe s� ruchy blokuj�ce
}


