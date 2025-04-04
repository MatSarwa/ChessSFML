#include "BoardView.h"
#include <iostream>

BoardView::BoardView(sf::RenderWindow& window, const ChessBoard& board)
    : window(&window), board(board), squareSize(75), boardPosition(0, 0),
    possibleMoves() {

    // £adowanie tekstury planszy
    if (!boardTexture.loadFromFile("resources/images/board.png")) {
        std::cerr << "Error loading board texture!" << std::endl;
    }

    // Ustaw wymiary planszy na 600x600
    float scale = 600.0f / static_cast<float>(boardTexture.getSize().x);
    boardSprite.setTexture(boardTexture);
    boardSprite.setScale(scale, scale);
    boardSprite.setPosition(boardPosition);

    // Dostosuj rozmiar pola do planszy
    squareSize = 600 / 8;

    // £adowanie tekstur figur
    // Bia³e figury
    if (!piecesTextures[0].loadFromFile("resources/images/king_w.png")) {
        std::cerr << "Error loading white king texture!" << std::endl;
    }
    if (!piecesTextures[1].loadFromFile("resources/images/queen_w.png")) {
        std::cerr << "Error loading white queen texture!" << std::endl;
    }
    if (!piecesTextures[2].loadFromFile("resources/images/rook_w.png")) {
        std::cerr << "Error loading white rook texture!" << std::endl;
    }
    if (!piecesTextures[3].loadFromFile("resources/images/bishop_w.png")) {
        std::cerr << "Error loading white bishop texture!" << std::endl;
    }
    if (!piecesTextures[4].loadFromFile("resources/images/knight_w.png")) {
        std::cerr << "Error loading white knight texture!" << std::endl;
    }
    if (!piecesTextures[5].loadFromFile("resources/images/pawn_w.png")) {
        std::cerr << "Error loading white pawn texture!" << std::endl;
    }

    // Czarne figury
    if (!piecesTextures[6].loadFromFile("resources/images/king_b.png")) {
        std::cerr << "Error loading black king texture!" << std::endl;
    }
    if (!piecesTextures[7].loadFromFile("resources/images/queen_b.png")) {
        std::cerr << "Error loading black queen texture!" << std::endl;
    }
    if (!piecesTextures[8].loadFromFile("resources/images/rook_b.png")) {
        std::cerr << "Error loading black rook texture!" << std::endl;
    }
    if (!piecesTextures[9].loadFromFile("resources/images/bishop_b.png")) {
        std::cerr << "Error loading black bishop texture!" << std::endl;
    }
    if (!piecesTextures[10].loadFromFile("resources/images/knight_b.png")) {
        std::cerr << "Error loading black knight texture!" << std::endl;
    }
    if (!piecesTextures[11].loadFromFile("resources/images/pawn_b.png")) {
        std::cerr << "Error loading black pawn texture!" << std::endl;
    }

    // Skalowanie tekstur figur do rozmiaru pola
    for (int i = 0; i < 12; i++) {
        // Ustawienie skali dla tekstur figur, aby pasowa³y do pola
        sf::Vector2u textureSize = piecesTextures[i].getSize();
        float pieceScale = static_cast<float>(squareSize) / static_cast<float>(textureSize.x);
        pieceScales[i] = pieceScale;
    }

    // Inicjalizacja podœwietleñ
    selectedPieceHighlight.setSize(sf::Vector2f(squareSize, squareSize));
    selectedPieceHighlight.setFillColor(sf::Color(255, 255, 0, 128)); // Pó³przezroczysty ¿ó³ty

    possibleMoveHighlight.setSize(sf::Vector2f(squareSize, squareSize));
    possibleMoveHighlight.setFillColor(sf::Color(0, 255, 0, 128)); // Pó³przezroczysty zielony

    std::cout << "BoardView initialized. Board position: " << boardPosition.x << "," << boardPosition.y
        << " Board size: 600x600 px, Square size: " << squareSize << " px" << std::endl;
}

void BoardView::render() {
    // Rysuj planszê
    window->draw(boardSprite);

    // Rysuj podœwietlenia dla mo¿liwych ruchów
    for (const auto& move : possibleMoves) {
        possibleMoveHighlight.setPosition(getScreenPosition(move.x, move.y));
        window->draw(possibleMoveHighlight);
    }

    // Rysuj figury
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            const Piece* piece = board.getPieceAt(row, col);
            if (piece && !piece->isEmpty()) {
                // U¿ywamy metody toInt() do konwersji figury na odpowiedni¹ wartoœæ liczbow¹
                int pieceValue = piece->toInt();
                sf::Sprite pieceSprite;

                int textureIndex = -1;
                // Wybierz odpowiedni¹ teksturê w zale¿noœci od typu figury
                if (pieceValue > 0) { // Bia³a figura
                    switch (pieceValue) {
                    case 4: textureIndex = 0; break; // Król
                    case 5: textureIndex = 1; break; // Królowa
                    case 1: textureIndex = 2; break; // Wie¿a
                    case 3: textureIndex = 3; break; // Goniec
                    case 2: textureIndex = 4; break; // Skoczek
                    case 6: textureIndex = 5; break; // Pionek
                    }
                }
                else if (pieceValue < 0) { // Czarna figura
                    switch (pieceValue) {
                    case -4: textureIndex = 6; break; // Król
                    case -5: textureIndex = 7; break; // Królowa
                    case -1: textureIndex = 8; break; // Wie¿a
                    case -3: textureIndex = 9; break; // Goniec
                    case -2: textureIndex = 10; break; // Skoczek
                    case -6: textureIndex = 11; break; // Pionek
                    }
                }

                if (textureIndex >= 0) {
                    pieceSprite.setTexture(piecesTextures[textureIndex]);
                    pieceSprite.setScale(pieceScales[textureIndex], pieceScales[textureIndex]);

                    // Wyœrodkuj figurê na polu
                    sf::FloatRect bounds = pieceSprite.getLocalBounds();
                    sf::Vector2f fieldPos = getScreenPosition(row, col);
                    pieceSprite.setPosition(
                        fieldPos.x + (squareSize - bounds.width * pieceScales[textureIndex]) / 2.0f,
                        fieldPos.y + (squareSize - bounds.height * pieceScales[textureIndex]) / 2.0f
                    );

                    window->draw(pieceSprite);
                }
            }
        }
    }
}

void BoardView::update() {
    // Aktualizacja stanu widoku (np. animacje)
}

sf::Vector2i BoardView::getBoardPosition(const sf::Vector2i& screenPos) const {
    // Konwersja pozycji ekranowej na wspó³rzêdne planszy
    float x = screenPos.x - boardPosition.x;
    float y = screenPos.y - boardPosition.y;

    // SprawdŸ, czy klikniêcie jest w granicach planszy
    if (x < 0 || x >= 8 * squareSize || y < 0 || y >= 8 * squareSize) {
        return sf::Vector2i(-1, -1); // Poza plansz¹
    }

    return sf::Vector2i(static_cast<int>(y / squareSize), static_cast<int>(x / squareSize));
}

sf::Vector2f BoardView::getScreenPosition(int row, int col) const {
    return sf::Vector2f(boardPosition.x + col * squareSize, boardPosition.y + row * squareSize);
}

void BoardView::highlightSelectedPiece(int row, int col) {
    selectedPieceHighlight.setPosition(getScreenPosition(row, col));
}

void BoardView::clearHighlights() {
    possibleMoves.clear();
}

void BoardView::setPossibleMoves(const std::vector<sf::Vector2i>& moves) {
    possibleMoves = moves;
}

void BoardView::animateMove(int fromRow, int fromCol, int toRow, int toCol) {
    // Implementacja animacji ruchu
}

void BoardView::showPromotionDialog(int row, int col, bool isWhite, std::function<void(int)> callback) {
    // Implementacja dialogu promocji pionka
}

sf::Vector2f BoardView::getBoardCenter() const {
    return sf::Vector2f(boardPosition.x + 4 * squareSize, boardPosition.y + 4 * squareSize);
}

float BoardView::getBoardWidth() const {
    return 8 * squareSize;
}

float BoardView::getBoardHeight() const {
    return 8 * squareSize;
}