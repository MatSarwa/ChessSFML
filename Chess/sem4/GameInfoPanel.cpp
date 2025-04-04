#include "GameInfoPanel.h"
#include <sstream>

GameInfoPanel::GameInfoPanel(sf::RenderWindow& win, const sf::Vector2f& pos, const sf::Vector2f& sz)
    : window(&win), position(pos), size(sz)
{
    // Inicjalizacja t³a
    background.setSize(size);
    background.setPosition(position);
    background.setFillColor(sf::Color(70, 70, 70));
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color(100, 100, 100));

    // Inicjalizacja tekstów
    font.loadFromFile("resources/fonts/arial.ttf"); // Spróbuj za³adowaæ czcionkê

    titleText.setFont(font);
    titleText.setString("Informacje");
    titleText.setCharacterSize(18);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(position.x + 10, position.y + 5);

    currentPlayerText.setFont(font);
    currentPlayerText.setString("Tura: Bia³y");
    currentPlayerText.setCharacterSize(14);
    currentPlayerText.setFillColor(sf::Color::White);
    currentPlayerText.setPosition(position.x + 10, position.y + 30);

    gameStateText.setFont(font);
    gameStateText.setString("");
    gameStateText.setCharacterSize(14);
    gameStateText.setFillColor(sf::Color::White);
    gameStateText.setPosition(position.x + 10, position.y + 50);
}

void GameInfoPanel::render() {
    window->draw(background);
    window->draw(titleText);
    window->draw(currentPlayerText);
    window->draw(gameStateText);

    // Renderowanie zbitych figur - uproszczone
    float offsetY = 80.0f;
    sf::Text capturedText;
    capturedText.setFont(font);
    capturedText.setString("Zbite figury:");
    capturedText.setCharacterSize(14);
    capturedText.setFillColor(sf::Color::White);
    capturedText.setPosition(position.x + 10, position.y + offsetY);
    window->draw(capturedText);
}

void GameInfoPanel::setCurrentPlayer(bool isWhite) {
    currentPlayerText.setString("Tura: " + std::string(isWhite ? "Bia³y" : "Czarny"));
}

void GameInfoPanel::setGameState(const std::string& state) {
    gameStateText.setString(state);
}

void GameInfoPanel::addCapturedPiece(int pieceValue) {
    // Uproszczona implementacja - tylko zliczanie zbitych figur
    CapturedPiece piece;
    piece.value = pieceValue;

    if (pieceValue > 0) {
        blackCaptured.push_back(piece);
    }
    else {
        whiteCaptured.push_back(piece);
    }
}

void GameInfoPanel::clearCapturedPieces() {
    whiteCaptured.clear();
    blackCaptured.clear();
}

int GameInfoPanel::getMaterialDifference() const {
    // Uproszczona implementacja - zwraca tylko ró¿nicê liczby zbitych figur
    return whiteCaptured.size() - blackCaptured.size();
}