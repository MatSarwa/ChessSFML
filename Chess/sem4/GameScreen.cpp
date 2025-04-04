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

    // £adowanie czcionki
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }

    // Inicjalizacja tekstu popupu
    popupText.setFont(font);
    popupText.setCharacterSize(24);
    popupText.setFillColor(sf::Color::White);

    // Ustaw jednolite kolory dla wszystkich przycisków
    sf::Color buttonColor(50, 80, 150);        // Ciemniejszy niebieski
    sf::Color hoverColor(100, 160, 255);       // Jaœniejszy niebieski przy najechaniu

    sf::Color textColor = sf::Color::White; 

    sf::Uint32 textStyle = sf::Text::Bold; // U¿ywaj pogrubienia dla lepszej widocznoœci
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

    // Inicjalizacja t³a
    background.setSize(sf::Vector2f(win.getSize().x, win.getSize().y));
    background.setFillColor(sf::Color(0x11, 0x2c, 0x49)); // Granatowe t³o

    // Inicjalizacja czasów 
    whiteTimer.reset(15 * 60); // 15 minut
    blackTimer.reset(15 * 60); // 15 minut  

    // Zresetuj grê na start
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
    // Kod wykonywany przy wyjœciu z ekranu gry
}

std::string GameScreen::handleEvent(const sf::Event& event) {
    // Najpierw sprawdŸ, czy popup jest widoczny
    if (showPopup) {
        // Jeœli popup jest widoczny, obs³uguj tylko jego zdarzenia
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
            if (popupOkButton.isClicked(mousePos)) {
                hidePopup();
                resetGame(); // Zresetuj grê po zamkniêciu popupu
            }
        }
        else if (event.type == sf::Event::MouseMoved) {
            // Obs³uga najechania mysz¹ na przycisk OK w popupie
            sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
            popupOkButton.update(mousePos);
        }

        // Gdy popup jest widoczny, nie przetwarzaj innych zdarzeñ
        return "";
    }

    // Pozosta³a obs³uga zdarzeñ (gdy popup nie jest widoczny)
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);

            // SprawdŸ czy klikniêto przyciski
            if (backButton.isClicked(mousePos)) {
                std::cout << "Button clicked: Return to menu" << std::endl;
                return "menu"; // To spowoduje powrót do menu w g³ównej pêtli
            }

            if (resetButton.isClicked(mousePos)) {
                resetGame();
                return "";
            }

            if (undoButton.isClicked(mousePos)) {
                undoLastMove();
                return "";
            }

            // Obs³uga klikniêcia na planszê
            if (!gameOver) {
                handleBoardClick(mousePos);
            }
        }
    }

    historyPanel.handleEvent(event);

    // Aktualizacja stanów przycisków
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

    // Co 60 wywo³añ (oko³o co sekundê przy 60 FPS) sprawdŸ stan timerów
    if (updateCounter >= 60) {
        updateCounter = 0;
    }

    boardView.update();

    // Aktualizacja zegarów
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

    // Rysowanie przycisków
    backButton.render(window);
    resetButton.render(window);
    undoButton.render(window);

    // Rysowanie zegarów
    whiteTimer.render();
    blackTimer.render();

    // Rysowanie panelu historii
    historyPanel.render();

    // Rysowanie popupu, jeœli jest widoczny
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
    currentPlayer = true; // Bia³e zaczynaj¹

    // Reset zegarów u¿ywaj¹cy zapisanych wartoœci czasów
    whiteTimer.reset(whitePlayerTime);
    blackTimer.reset(blackPlayerTime);

    // Zatrzymaj oba timery - nie uruchamiaj jeszcze bia³ego zegara,
    // zostanie on uruchomiony po pierwszym ruchu bia³ych
    whiteTimer.stop();
    blackTimer.stop();

    // Reset panelu historii
    historyPanel.clear();

    std::cout << "New game! White's move." << std::endl;

    // Wyczyœæ podœwietlenia
    boardView.clearHighlights();
}

void GameScreen::handleBoardClick(const sf::Vector2i& mousePos) {
    sf::Vector2i boardPos = boardView.getBoardPosition(mousePos);

    // SprawdŸ, czy klikniêcie jest w granicach planszy
    if (boardPos.x == -1 || boardPos.y == -1) {
        return;
    }

    const Piece* clickedPiece = chessBoard.getPieceAt(boardPos.x, boardPos.y);
    bool isCurrentPlayerPiece = false;

    // SprawdŸ czy klikniêto figurê aktualnego gracza
    if (clickedPiece) {
        isCurrentPlayerPiece = !clickedPiece->isEmpty() &&
            ((currentPlayer && clickedPiece->isWhite()) ||
                (!currentPlayer && clickedPiece->isBlack()));
    }

    if (!isPieceSelected) {
        // Jeœli nie wybrano figury, a klikniêto figurê aktualnego gracza
        if (isCurrentPlayerPiece) {
            isPieceSelected = true;
            selectedPiecePos = boardPos;
            boardView.highlightSelectedPiece(boardPos.x, boardPos.y);

            // Poka¿ mo¿liwe ruchy
            auto moves = chessBoard.getPossibleMoves(boardPos.x, boardPos.y);
            boardView.setPossibleMoves(moves);
        }
    }
    else {
        // Jeœli ju¿ wybrano figurê
        if (boardPos.x == selectedPiecePos.x && boardPos.y == selectedPiecePos.y) {
            // Odznacz figurê, jeœli klikniêto j¹ ponownie
            isPieceSelected = false;
            boardView.clearHighlights();
        }
        else if (isCurrentPlayerPiece) {
            // Wybierz inn¹ figurê aktualnego gracza
            selectedPiecePos = boardPos;
            boardView.highlightSelectedPiece(boardPos.x, boardPos.y);

            // Poka¿ mo¿liwe ruchy
            auto moves = chessBoard.getPossibleMoves(boardPos.x, boardPos.y);
            boardView.setPossibleMoves(moves);
        }
        else {
            // SprawdŸ, czy ruch jest dozwolony
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

        // Zapisz notacjê ruchu
        std::string moveNotation = generateMoveNotation(fromRow, fromCol, toRow, toCol);

        // Tymczasowo zmieñ gracza, aby sprawdziæ czy ruch spowodowa³ szach/mat dla przeciwnika
        currentPlayer = !currentPlayer;

        // SprawdŸ czy ruch spowodowa³ szach lub szach-mat dla nowego aktualnego gracza
        bool isCheck = chessBoard.isInCheck(currentPlayer);
        bool isCheckmate = chessBoard.isCheckmate(currentPlayer);

        // Przywróæ oryginalnego gracza
        currentPlayer = !currentPlayer;

        // Dodaj ruch do panelu historii z informacj¹ o szachu/macie
        ChessMove move(moveNotation, currentPlayer, isCheck, isCheckmate);
        historyPanel.addMove(move);

        std::cout << "Move: " << moveNotation << std::endl;

        // SprawdŸ czy pionek wymaga promocji
        if (needsPromotion(toRow, toCol)) {
            // Poka¿ dialog promocji
            boardView.showPromotionDialog(toRow, toCol, currentPlayer, [this, toRow, toCol](int pieceType) {
                // Promocja pionka na wybran¹ figurê
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

                // Zatrzymaj zegar obecnego gracza i uruchom zegar nastêpnego gracza
                if (currentPlayer) {
                    whiteTimer.stop();
                    blackTimer.start();
                }
                else {
                    blackTimer.stop();
                    whiteTimer.start();
                }

                // Zmieñ gracza
                currentPlayer = !currentPlayer;

                // Aktualizuj informacje
                std::cout << (currentPlayer ? "White's move" : "Black's move") << std::endl;

                // SprawdŸ stan gry
                checkGameState();
                });
        }
        else {
            // Standardowa kontynuacja po ruchu
            isPieceSelected = false;
            boardView.clearHighlights();

            // Zatrzymaj zegar obecnego gracza i uruchom zegar nastêpnego gracza
            if (currentPlayer) {
                whiteTimer.stop();
                blackTimer.start();
            }
            else {
                blackTimer.stop();
                whiteTimer.start();
            }

            // Zmieñ gracza
            currentPlayer = !currentPlayer;

            // Aktualizuj informacje
            std::cout << (currentPlayer ? "White's move" : "Black's move") << std::endl;

            // SprawdŸ stan gry
            checkGameState();
        }
    }
}

bool GameScreen::needsPromotion(int row, int col) {
    const Piece* piece = chessBoard.getPieceAt(row, col);

    if (piece && piece->getType() == PieceType::PAWN) {
        // Pionek na ostatnim rzêdzie
        if ((piece->isWhite() && row == 0) || (piece->isBlack() && row == 7)) {
            return true;
        }
    }

    return false;
}

std::string GameScreen::generateMoveNotation(int fromRow, int fromCol, int toRow, int toCol) {
    // Implementacja generowania notacji algebraicznej szachów
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

    // Dodaj wspó³rzêdne startowe
    notation += files[fromCol];
    notation += ranks[fromRow];

    // Dodaj symbol bicia
    // Pobierz figurê z pozycji docelowej przed wykonaniem ruchu
    const Piece* targetPiece = chessBoard.getPieceAt(toRow, toCol);
    if (targetPiece && !targetPiece->isEmpty()) {
        notation += "x";
    }
    else {
        notation += "-";
    }

    // Dodaj wspó³rzêdne docelowe
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
        // Jeœli gra siê skoñczy³a lub nie ma ruchów do cofniêcia
        return;
    }

    // Pobierz ostatni ruch z panelu historii
    const ChessMove& lastMove = historyPanel.getLastMove();

    // Parsuj notacjê, by uzyskaæ wspó³rzêdne
    // Format notacji to np. "Qe2-e4" lub "Pe2xe4"
    // Musimy wyci¹gn¹æ wspó³rzêdne e2 (sk¹d) i e4 (dok¹d)
    std::string notation = lastMove.notation;

    // Pomiñ symbol figury (K, Q, R, B, N) jeœli jest
    int startIndex = 0;
    if (notation[0] >= 'A' && notation[0] <= 'Z') {
        startIndex = 1;
    }

    // Pobierz wspó³rzêdne startowe (np. e2)
    char fromFile = notation[startIndex];      // e
    char fromRank = notation[startIndex + 1];  // 2

    // Pobierz wspó³rzêdne koñcowe (np. e4)
    // ZnajdŸ indeks po symbolu ruchu (-) lub bicia (x)
    int toIndex = notation.find('-');
    if (toIndex == std::string::npos) {
        toIndex = notation.find('x');
    }
    toIndex += 1; // Przeskocz symbol ruchu

    char toFile = notation[toIndex];      // e
    char toRank = notation[toIndex + 1];  // 4

    // Konwertuj wspó³rzêdne na indeksy w tablicy board
    int fromCol = fromFile - 'a';
    int fromRow = '8' - fromRank;
    int toCol = toFile - 'a';
    int toRow = '8' - toRank;

    // Wykonaj operacjê cofniêcia ruchu
    chessBoard.makeMove(toRow, toCol, fromRow, fromCol); // Przywróæ figurê

    // Usuñ ostatni ruch z panelu historii
    historyPanel.removeLastMove();

    // Zmieñ aktualnego gracza
    currentPlayer = !currentPlayer;

    // Prze³¹cz zegary
    if (currentPlayer) {
        blackTimer.stop();
        whiteTimer.start();
    }
    else {
        whiteTimer.stop();
        blackTimer.start();
    }

    // Wyczyœæ zaznaczenia
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
    popupText.setCharacterSize(24); // Wiêkszy rozmiar tekstu

    // Dostosuj po³o¿enie i rozmiar popupu do okna
    sf::Vector2u windowSize = window.getSize();
    float popupWidth = 400.0f;
    float popupHeight = 200.0f;

    // T³o popupu - ciemny granatowy kolor
    popupBackground.setSize(sf::Vector2f(popupWidth, popupHeight));
    popupBackground.setPosition((windowSize.x - popupWidth) / 2, (windowSize.y - popupHeight) / 2);
    popupBackground.setFillColor(sf::Color(20, 30, 70, 230)); // Ciemny granatowy z przezroczystoœci¹
    popupBackground.setOutlineThickness(3);
    popupBackground.setOutlineColor(color);

    // Wyœrodkuj tekst
    sf::FloatRect textBounds = popupText.getLocalBounds();
    popupText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    popupText.setPosition(
        windowSize.x / 2,
        (windowSize.y / 2) - 30 // Przesuñ tekst nieco wy¿ej, aby zrobiæ miejsce na przycisk
    );

    // Ustawienie przycisku OK
    popupOkButton.setText("OK");

    // Ustaw przycisk OK na œrodku, pod tekstem
    float buttonX = (windowSize.x / 2) - 50; // Centruj przycisk (50 to po³owa standardowej szerokoœci)
    float buttonY = (windowSize.y / 2) + 30; // Pod tekstem
    popupOkButton.setPosition(buttonX, buttonY);

    // Zmiana kolorów przycisku na bardziej dopasowane do tematu
    sf::Color buttonColor(50, 80, 150); // Ciemniejszy niebieski
    sf::Color hoverColor(100, 160, 255); // Jaœniejszy, bardziej nasycony niebieski przy najechaniu
    popupOkButton.setColors(buttonColor, hoverColor);
}

void GameScreen::hidePopup() {
    showPopup = false;
}