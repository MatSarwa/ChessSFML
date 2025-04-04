#include "GameScreen.h"
#include <iostream>

GameScreen::GameScreen(sf::RenderWindow& win, ApplicationManager* manager) : Screen(win),
boardView(win, chessBoard),
backButton(10, boardView.getBoardHeight() + 650, 150, 40, "Return to menu", 16),
resetButton(170, boardView.getBoardHeight() + 650, 150, 40, "Reset game", 16),
undoButton(330, boardView.getBoardHeight() + 650, 150, 40, "Undo move", 16),
whiteTimer(win, sf::Vector2f(boardView.getBoardWidth() + 100, 100), sf::Vector2f(200, 80), true),
blackTimer(win, sf::Vector2f(boardView.getBoardWidth() + 100, 200), sf::Vector2f(200, 80), false),
historyPanel(win, sf::Vector2f(boardView.getBoardWidth() + 100, 300), sf::Vector2f(200, 300)),
isPieceSelected(false),
selectedPiecePos(-1, -1),
gameOver(false),
currentPlayer(true),
showPopup(false),
popupOkButton(0, 0, 100, 40, "OK", 18),
appManager(manager) {

    // �adowanie czcionki
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }

    // Inicjalizacja tekstu popupu
    popupText.setFont(font);
    popupText.setCharacterSize(24);
    popupText.setFillColor(sf::Color::White);

    // Ustaw jednolite kolory dla wszystkich przycisk�w
    sf::Color buttonColor(50, 80, 150);        // Ciemniejszy niebieski
    sf::Color hoverColor(100, 160, 255);       // Ja�niejszy niebieski przy najechaniu

    sf::Color textColor = sf::Color::White; 

    sf::Uint32 textStyle = sf::Text::Bold; // U�ywaj pogrubienia dla lepszej widoczno�ci
    backButton.setTextStyle(textStyle);
    resetButton.setTextStyle(textStyle);
    undoButton.setTextStyle(textStyle);

    backButton.setColors(buttonColor, hoverColor);
    backButton.setFont(font);
    backButton.setTextColor(textColor);

    resetButton.setColors(buttonColor, hoverColor);
    resetButton.setFont(font);
    resetButton.setTextColor(textColor);

    undoButton.setColors(buttonColor, hoverColor);
    undoButton.setFont(font);
    undoButton.setTextColor(textColor);

    popupOkButton.setColors(buttonColor, hoverColor);

    // Inicjalizacja t�a
    background.setSize(sf::Vector2f(win.getSize().x, win.getSize().y));
    background.setFillColor(sf::Color(0x11, 0x2c, 0x49)); // Granatowe t�o

    // Inicjalizacja czas�w 
    whiteTimer.reset(15 * 60); // 15 minut
    blackTimer.reset(15 * 60); // 15 minut  

    // Zresetuj gr� na start
    resetGame();
}

void GameScreen::onEnter() {
    if (appManager) {
        int whiteTime = appManager->getWhitePlayerTime();
        int blackTime = appManager->getBlackPlayerTime();

        // Ustaw czasy w timerach
        whiteTimer.setRemainingTime(whiteTime);
        blackTimer.setRemainingTime(blackTime);
    }

    updateBackgroundSize();
}

void GameScreen::onExit() {
    // Kod wykonywany przy wyj�ciu z ekranu gry
}

std::string GameScreen::handleEvent(const sf::Event& event) {
    // Najpierw sprawd�, czy popup jest widoczny
    if (showPopup) {
        // Je�li popup jest widoczny, obs�uguj tylko jego zdarzenia
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
            if (popupOkButton.isClicked(mousePos)) {
                hidePopup();
                resetGame(); // Zresetuj gr� po zamkni�ciu popupu
            }
        }
        else if (event.type == sf::Event::MouseMoved) {
            // Obs�uga najechania mysz� na przycisk OK w popupie
            sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
            popupOkButton.update(mousePos);
        }

        // Gdy popup jest widoczny, nie przetwarzaj innych zdarze�
        return "";
    }

    // Pozosta�a obs�uga zdarze� (gdy popup nie jest widoczny)
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);

            // Sprawd� czy klikni�to przyciski
            if (backButton.isClicked(mousePos)) {
                std::cout << "Button clicked: Return to menu" << std::endl;
                return "menu"; // To spowoduje powr�t do menu w g��wnej p�tli
            }

            if (resetButton.isClicked(mousePos)) {
                resetGame();
                return "";
            }

            if (undoButton.isClicked(mousePos)) {
                undoLastMove();
                return "";
            }

            // Obs�uga klikni�cia na plansz�
            if (!gameOver) {
                handleBoardClick(mousePos);
            }
        }
    }

    historyPanel.handleEvent(event);

    // Aktualizacja stan�w przycisk�w
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
        backButton.update(mousePos);
        resetButton.update(mousePos);
        undoButton.update(mousePos);
    }

    return "";
}

void GameScreen::update() {
    // Aktualizacja licznika
    static int updateCounter = 0;
    updateCounter++;

    // Co 60 wywo�a� (oko�o co sekund� przy 60 FPS) sprawd� stan timer�w
    if (updateCounter >= 60) {
        updateCounter = 0;
    }

    boardView.update();

    // Aktualizacja zegar�w
    whiteTimer.update();
    blackTimer.update();

    if (!gameOver) {
        if (currentPlayer) {
            if (whiteTimer.getRemainingTime() <= 0) {
                gameOver = true;
                std::string message = "Time's up! Black wins!";
                showPopupWin(message, sf::Color::Black);
                std::cout << message << std::endl;
            }
        }
        else {
            if (blackTimer.getRemainingTime() <= 0) {
                gameOver = true;
                std::string message = "Time's up! White wins!";
                showPopupWin(message, sf::Color::White);
                std::cout << message << std::endl;
            }
        }
    }
}

void GameScreen::render() {
    window.draw(background);
    boardView.render();

    // Rysowanie przycisk�w
    backButton.render(window);
    resetButton.render(window);
    undoButton.render(window);

    // Rysowanie zegar�w
    whiteTimer.render();
    blackTimer.render();

    // Rysowanie panelu historii
    historyPanel.render();

    // Rysowanie popupu, je�li jest widoczny
    if (showPopup) {
        window.draw(popupBackground);
        window.draw(popupText);
        popupOkButton.render(window);
    }
}

void GameScreen::resetGame() {
    chessBoard.resetBoard();

    isPieceSelected = false;
    selectedPiecePos = sf::Vector2i(-1, -1);
    gameOver = false;
    currentPlayer = true; // Bia�e zaczynaj�

    // Reset zegar�w u�ywaj�cy zapisanych warto�ci czas�w
    whiteTimer.reset(whitePlayerTime);
    blackTimer.reset(blackPlayerTime);

    // Zatrzymaj oba timery - nie uruchamiaj jeszcze bia�ego zegara,
    // zostanie on uruchomiony po pierwszym ruchu bia�ych
    whiteTimer.stop();
    blackTimer.stop();

    // Reset panelu historii
    historyPanel.clear();

    std::cout << "New game! White's move." << std::endl;

    // Wyczy�� pod�wietlenia
    boardView.clearHighlights();
}

void GameScreen::handleBoardClick(const sf::Vector2i& mousePos) {
    sf::Vector2i boardPos = boardView.getBoardPosition(mousePos);

    // Sprawd�, czy klikni�cie jest w granicach planszy
    if (boardPos.x == -1 || boardPos.y == -1) {
        return;
    }

    const Piece* clickedPiece = chessBoard.getPieceAt(boardPos.x, boardPos.y);
    bool isCurrentPlayerPiece = false;

    // Sprawd� czy klikni�to figur� aktualnego gracza
    if (clickedPiece) {
        isCurrentPlayerPiece = !clickedPiece->isEmpty() &&
            ((currentPlayer && clickedPiece->isWhite()) ||
                (!currentPlayer && clickedPiece->isBlack()));
    }

    if (!isPieceSelected) {
        // Je�li nie wybrano figury, a klikni�to figur� aktualnego gracza
        if (isCurrentPlayerPiece) {
            isPieceSelected = true;
            selectedPiecePos = boardPos;
            boardView.highlightSelectedPiece(boardPos.x, boardPos.y);

            // Poka� mo�liwe ruchy
            auto moves = chessBoard.getPossibleMoves(boardPos.x, boardPos.y);
            boardView.setPossibleMoves(moves);
        }
    }
    else {
        // Je�li ju� wybrano figur�
        if (boardPos.x == selectedPiecePos.x && boardPos.y == selectedPiecePos.y) {
            // Odznacz figur�, je�li klikni�to j� ponownie
            isPieceSelected = false;
            boardView.clearHighlights();
        }
        else if (isCurrentPlayerPiece) {
            // Wybierz inn� figur� aktualnego gracza
            selectedPiecePos = boardPos;
            boardView.highlightSelectedPiece(boardPos.x, boardPos.y);

            // Poka� mo�liwe ruchy
            auto moves = chessBoard.getPossibleMoves(boardPos.x, boardPos.y);
            boardView.setPossibleMoves(moves);
        }
        else {
            // Sprawd�, czy ruch jest dozwolony
            auto moves = chessBoard.getPossibleMoves(selectedPiecePos.x, selectedPiecePos.y);
            bool validMove = false;

            for (const auto& move : moves) {
                if (move.x == boardPos.x && move.y == boardPos.y) {
                    validMove = true;
                    break;
                }
            }

            if (validMove) {
                makeMove(selectedPiecePos.x, selectedPiecePos.y, boardPos.x, boardPos.y);
            }
        }
    }
}

void GameScreen::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    // Wykonaj ruch w modelu
    bool success = chessBoard.makeMove(fromRow, fromCol, toRow, toCol);

    if (success) {
        // Animuj ruch
        boardView.animateMove(fromRow, fromCol, toRow, toCol);

        // Zapisz notacj� ruchu
        std::string moveNotation = generateMoveNotation(fromRow, fromCol, toRow, toCol);

        // Tymczasowo zmie� gracza, aby sprawdzi� czy ruch spowodowa� szach/mat dla przeciwnika
        currentPlayer = !currentPlayer;

        // Sprawd� czy ruch spowodowa� szach lub szach-mat dla nowego aktualnego gracza
        bool isCheck = chessBoard.isInCheck(currentPlayer);
        bool isCheckmate = chessBoard.isCheckmate(currentPlayer);

        // Przywr�� oryginalnego gracza
        currentPlayer = !currentPlayer;

        // Dodaj ruch do panelu historii z informacj� o szachu/macie
        ChessMove move(moveNotation, currentPlayer, isCheck, isCheckmate);
        historyPanel.addMove(move);

        std::cout << "Move: " << moveNotation << std::endl;

        // Sprawd� czy pionek wymaga promocji
        if (needsPromotion(toRow, toCol)) {
            // Poka� dialog promocji
            boardView.showPromotionDialog(toRow, toCol, currentPlayer, [this, toRow, toCol](int pieceType) {
                // Promocja pionka na wybran� figur�
                PieceType newType;
                switch (pieceType) {
                case 5: newType = PieceType::QUEEN; break;
                case 1: newType = PieceType::ROOK; break;
                case 3: newType = PieceType::BISHOP; break;
                case 2: newType = PieceType::KNIGHT; break;
                default: newType = PieceType::QUEEN; break;
                }
                chessBoard.promotePawn(toRow, toCol, newType);

                // Kontynuuj po promocji
                isPieceSelected = false;
                boardView.clearHighlights();

                // Zatrzymaj zegar obecnego gracza i uruchom zegar nast�pnego gracza
                if (currentPlayer) {
                    whiteTimer.stop();
                    blackTimer.start();
                }
                else {
                    blackTimer.stop();
                    whiteTimer.start();
                }

                // Zmie� gracza
                currentPlayer = !currentPlayer;

                // Aktualizuj informacje
                std::cout << (currentPlayer ? "White's move" : "Black's move") << std::endl;

                // Sprawd� stan gry
                checkGameState();
                });
        }
        else {
            // Standardowa kontynuacja po ruchu
            isPieceSelected = false;
            boardView.clearHighlights();

            // Zatrzymaj zegar obecnego gracza i uruchom zegar nast�pnego gracza
            if (currentPlayer) {
                whiteTimer.stop();
                blackTimer.start();
            }
            else {
                blackTimer.stop();
                whiteTimer.start();
            }

            // Zmie� gracza
            currentPlayer = !currentPlayer;

            // Aktualizuj informacje
            std::cout << (currentPlayer ? "White's move" : "Black's move") << std::endl;

            // Sprawd� stan gry
            checkGameState();
        }
    }
}

bool GameScreen::needsPromotion(int row, int col) {
    const Piece* piece = chessBoard.getPieceAt(row, col);

    if (piece && piece->getType() == PieceType::PAWN) {
        // Pionek na ostatnim rz�dzie
        if ((piece->isWhite() && row == 0) || (piece->isBlack() && row == 7)) {
            return true;
        }
    }

    return false;
}

std::string GameScreen::generateMoveNotation(int fromRow, int fromCol, int toRow, int toCol) {
    // Implementacja generowania notacji algebraicznej szach�w
    char files[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    char ranks[] = { '8', '7', '6', '5', '4', '3', '2', '1' };

    const Piece* piece = chessBoard.getPieceAt(toRow, toCol); // Figura po ruchu

    std::string notation;

    // Dodaj symbol figury (dla pionka pomijamy)
    if (piece) {
        switch (piece->getType()) {
        case PieceType::KING: notation += "K"; break;
        case PieceType::QUEEN: notation += "Q"; break;
        case PieceType::ROOK: notation += "R"; break;
        case PieceType::BISHOP: notation += "B"; break;
        case PieceType::KNIGHT: notation += "N"; break;
        default: break; // Pionek nie ma specjalnego oznaczenia
        }
    }

    // Dodaj wsp�rz�dne startowe
    notation += files[fromCol];
    notation += ranks[fromRow];

    // Dodaj symbol bicia
    // Pobierz figur� z pozycji docelowej przed wykonaniem ruchu
    const Piece* targetPiece = chessBoard.getPieceAt(toRow, toCol);
    if (targetPiece && !targetPiece->isEmpty()) {
        notation += "x";
    }
    else {
        notation += "-";
    }

    // Dodaj wsp�rz�dne docelowe
    notation += files[toCol];
    notation += ranks[toRow];

    return notation;
}

void GameScreen::checkGameState() {
    if (chessBoard.isCheckmate(currentPlayer)) {
        gameOver = true;
        std::string message = currentPlayer ? "Checkmate! Black wins!" : "Checkmate! White wins!";
        sf::Color color = currentPlayer ? sf::Color::Black : sf::Color::White;
        std::cout << message << std::endl;
        showPopupWin(message, color);
    }
    else if (chessBoard.isStalemate(currentPlayer)) {
        gameOver = true;
        std::string message = "Stalemate! Draw!";
        sf::Color color = sf::Color(150, 150, 150); // Szary dla remisu
        std::cout << message << std::endl;
        showPopupWin(message, color);
    }
    else if (chessBoard.isInCheck(currentPlayer)) {
        // Tylko komunikat o szachu
        std::cout << (currentPlayer ? "Check for White!" : "Check for Black!") << std::endl;
    }
}

void GameScreen::undoLastMove() {

    if (gameOver || historyPanel.getMoves().empty()) {
        // Je�li gra si� sko�czy�a lub nie ma ruch�w do cofni�cia
        return;
    }

    // Pobierz ostatni ruch z panelu historii
    const ChessMove& lastMove = historyPanel.getLastMove();

    // Parsuj notacj�, by uzyska� wsp�rz�dne
    // Format notacji to np. "Qe2-e4" lub "Pe2xe4"
    // Musimy wyci�gn�� wsp�rz�dne e2 (sk�d) i e4 (dok�d)
    std::string notation = lastMove.notation;

    // Pomi� symbol figury (K, Q, R, B, N) je�li jest
    int startIndex = 0;
    if (notation[0] >= 'A' && notation[0] <= 'Z') {
        startIndex = 1;
    }

    // Pobierz wsp�rz�dne startowe (np. e2)
    char fromFile = notation[startIndex];      // e
    char fromRank = notation[startIndex + 1];  // 2

    // Pobierz wsp�rz�dne ko�cowe (np. e4)
    // Znajd� indeks po symbolu ruchu (-) lub bicia (x)
    int toIndex = notation.find('-');
    if (toIndex == std::string::npos) {
        toIndex = notation.find('x');
    }
    toIndex += 1; // Przeskocz symbol ruchu

    char toFile = notation[toIndex];      // e
    char toRank = notation[toIndex + 1];  // 4

    // Konwertuj wsp�rz�dne na indeksy w tablicy board
    int fromCol = fromFile - 'a';
    int fromRow = '8' - fromRank;
    int toCol = toFile - 'a';
    int toRow = '8' - toRank;

    // Wykonaj operacj� cofni�cia ruchu
    chessBoard.makeMove(toRow, toCol, fromRow, fromCol); // Przywr�� figur�

    // Usu� ostatni ruch z panelu historii
    historyPanel.removeLastMove();

    // Zmie� aktualnego gracza
    currentPlayer = !currentPlayer;

    // Prze��cz zegary
    if (currentPlayer) {
        blackTimer.stop();
        whiteTimer.start();
    }
    else {
        whiteTimer.stop();
        blackTimer.start();
    }

    // Wyczy�� zaznaczenia
    isPieceSelected = false;
    boardView.clearHighlights();

    std::cout << (currentPlayer ? "White's move" : "Black's move") << " (after undo)" << std::endl;
}

void GameScreen::updateBackgroundSize() {
    background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
}

void GameScreen::setPlayerTimes(int whiteTime, int blackTime) {
    whitePlayerTime = whiteTime;
    blackPlayerTime = blackTime;

    // Od razu ustaw czasy w zegarach
    whiteTimer.reset(whiteTime);
    blackTimer.reset(blackTime);

    // Zatrzymaj oba zegary - pierwszy zostanie uruchomiony po ruchu
    whiteTimer.stop();
    blackTimer.stop();

    std::cout << "Set player times: White: " << whiteTime << "s, Black: " << blackTime << "s" << std::endl;
}

void GameScreen::showPopupWin(const std::string& message, sf::Color color) {
    showPopup = true;
    popupMessage = message;
    popupColor = color;

    // Ustawienie tekstu
    popupText.setString(message);
    popupText.setFillColor(sf::Color::White); // Jasny tekst na ciemnym tle
    popupText.setCharacterSize(24); // Wi�kszy rozmiar tekstu

    // Dostosuj po�o�enie i rozmiar popupu do okna
    sf::Vector2u windowSize = window.getSize();
    float popupWidth = 400.0f;
    float popupHeight = 200.0f;

    // T�o popupu - ciemny granatowy kolor
    popupBackground.setSize(sf::Vector2f(popupWidth, popupHeight));
    popupBackground.setPosition((windowSize.x - popupWidth) / 2, (windowSize.y - popupHeight) / 2);
    popupBackground.setFillColor(sf::Color(20, 30, 70, 230)); // Ciemny granatowy z przezroczysto�ci�
    popupBackground.setOutlineThickness(3);
    popupBackground.setOutlineColor(color);

    // Wy�rodkuj tekst
    sf::FloatRect textBounds = popupText.getLocalBounds();
    popupText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    popupText.setPosition(
        windowSize.x / 2,
        (windowSize.y / 2) - 30 // Przesu� tekst nieco wy�ej, aby zrobi� miejsce na przycisk
    );

    // Ustawienie przycisku OK
    popupOkButton.setText("OK");

    // Ustaw przycisk OK na �rodku, pod tekstem
    float buttonX = (windowSize.x / 2) - 50; // Centruj przycisk (50 to po�owa standardowej szeroko�ci)
    float buttonY = (windowSize.y / 2) + 30; // Pod tekstem
    popupOkButton.setPosition(buttonX, buttonY);

    // Zmiana kolor�w przycisku na bardziej dopasowane do tematu
    sf::Color buttonColor(50, 80, 150); // Ciemniejszy niebieski
    sf::Color hoverColor(100, 160, 255); // Ja�niejszy, bardziej nasycony niebieski przy najechaniu
    popupOkButton.setColors(buttonColor, hoverColor);
}

void GameScreen::hidePopup() {
    showPopup = false;
}