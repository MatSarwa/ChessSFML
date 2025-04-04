#include "MoveHistoryPanel.h"
#include <iostream>
#include <algorithm>

MoveHistoryPanel::MoveHistoryPanel(sf::RenderWindow& win, const sf::Vector2f& pos, const sf::Vector2f& sz)
    : window(&win),
    position(pos),
    size(sz),
    scrollOffset(0),
    maxVisibleMoves(10),
    isDraggingScrollbar(false)
{
    // Inicjalizacja t³a
    background.setSize(size);
    background.setPosition(position);
    background.setFillColor(sf::Color(70, 70, 70, 200)); // Pó³przezroczyste t³o
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color(100, 100, 100));

    // Inicjalizacja tekstów
    if (!font.loadFromFile("resources/fonts/arial.ttf")) { // U¿yj tej samej czcionki co reszta interfejsu
        std::cerr << "Error loading font in MoveHistoryPanel!" << std::endl;
    }

    titleText.setFont(font);
    titleText.setString("Move History");
    titleText.setCharacterSize(18);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(position.x + 10, position.y + 5);

    // Inicjalizacja scrollbara
    scrollbar.setSize(sf::Vector2f(15, size.y - 40));
    scrollbar.setPosition(position.x + size.x - 20, position.y + 35);
    scrollbar.setFillColor(sf::Color(50, 50, 50));

    scrollbarThumb.setSize(sf::Vector2f(15, 40));
    scrollbarThumb.setPosition(position.x + size.x - 20, position.y + 35);
    scrollbarThumb.setFillColor(sf::Color(120, 120, 120));
}

void MoveHistoryPanel::render() {
    window->draw(background);
    window->draw(titleText);

    // Rysowanie listy ruchów
    float offsetY = 35.0f;
    for (int i = scrollOffset; i < std::min(scrollOffset + maxVisibleMoves, static_cast<int>(moves.size())); i++) {
        sf::Text moveText;
        moveText.setFont(font);

        std::string text = std::to_string(i / 2 + 1) + ". ";
        if (i % 2 == 1) {
            text = "   "; // Wciêcie dla ruchów czarnych
        }

        text += moves[i].notation;

        if (moves[i].isCheck) {
            text += "+";
        }
        if (moves[i].isCheckmate) {
            text += "#";
        }

        moveText.setString(text);
        moveText.setCharacterSize(14);
        moveText.setFillColor(moves[i].isWhiteMove ? sf::Color::White : sf::Color(200, 200, 200));
        moveText.setPosition(position.x + 10, position.y + offsetY);

        window->draw(moveText);
        offsetY += 20.0f;
    }

    // Rysowanie scrollbara (jeœli potrzebny)
    if (moves.size() > maxVisibleMoves) {
        window->draw(scrollbar);
        window->draw(scrollbarThumb);
    }
}

void MoveHistoryPanel::addMove(const ChessMove& move) {
    moves.push_back(move);

    // Automatyczne przewijanie do ostatniego ruchu
    if (moves.size() > maxVisibleMoves) {
        scrollOffset = static_cast<int>(moves.size()) - maxVisibleMoves;

        // Aktualizacja pozycji scrollbara
        if (moves.size() > maxVisibleMoves) {
            float thumbPosition = scrollOffset * (scrollbar.getSize().y - scrollbarThumb.getSize().y) / (moves.size() - maxVisibleMoves);
            scrollbarThumb.setPosition(scrollbarThumb.getPosition().x, scrollbar.getPosition().y + thumbPosition);
        }
    }
}

const std::vector<ChessMove>& MoveHistoryPanel::getMoves() const {
    return moves;
}

const ChessMove& MoveHistoryPanel::getLastMove() const {
    // Zabezpieczenie przed pustym wektorem
    static ChessMove emptyMove("", false);
    if (moves.empty()) {
        return emptyMove;
    }
    return moves.back();
}

void MoveHistoryPanel::removeLastMove() {
    if (!moves.empty()) {
        moves.pop_back();

        // Aktualizacja offsetu przewijania, jeœli potrzebna
        if (scrollOffset > 0 && moves.size() <= scrollOffset + maxVisibleMoves) {
            scrollOffset = std::max(0, static_cast<int>(moves.size()) - maxVisibleMoves);

            // Aktualizacja pozycji scrollbara
            if (moves.size() > maxVisibleMoves) {
                float scrollRatio = static_cast<float>(scrollOffset) / (moves.size() - maxVisibleMoves);
                float minY = scrollbar.getPosition().y;
                float maxY = scrollbar.getPosition().y + scrollbar.getSize().y - scrollbarThumb.getSize().y;
                float newY = minY + scrollRatio * (maxY - minY);

                scrollbarThumb.setPosition(scrollbarThumb.getPosition().x, newY);
            }
            else {
                // Jeœli nie ma wystarczaj¹co ruchów dla scrollbara, zresetuj go
                scrollbarThumb.setPosition(scrollbar.getPosition().x, scrollbar.getPosition().y);
            }
        }
    }
}

void MoveHistoryPanel::clear() {
    moves.clear();
    scrollOffset = 0;
    scrollbarThumb.setPosition(scrollbar.getPosition().x, scrollbar.getPosition().y);
}

void MoveHistoryPanel::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);

        // Sprawdzenie czy klikniêto scrollbar
        if (scrollbarThumb.getGlobalBounds().contains(mousePos)) {
            isDraggingScrollbar = true;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDraggingScrollbar = false;
    }
    else if (event.type == sf::Event::MouseMoved && isDraggingScrollbar) {
        // Przesuwanie scrollbara
        float minY = scrollbar.getPosition().y;
        float maxY = scrollbar.getPosition().y + scrollbar.getSize().y - scrollbarThumb.getSize().y;
        float newY = std::max(minY, std::min(maxY, static_cast<float>(event.mouseMove.y) - scrollbarThumb.getSize().y / 2.0f));

        scrollbarThumb.setPosition(scrollbarThumb.getPosition().x, newY);

        // Aktualizacja offsetu przewijania
        if (moves.size() > maxVisibleMoves) {
            float scrollRatio = (newY - minY) / (maxY - minY);
            scrollOffset = static_cast<int>(scrollRatio * (moves.size() - maxVisibleMoves));
        }
    }
    else if (event.type == sf::Event::MouseWheelScrolled) {
        // Obs³uga kó³ka myszy
        scroll(-event.mouseWheelScroll.delta);
    }
}

void MoveHistoryPanel::scroll(int delta) {
    if (moves.size() <= maxVisibleMoves) {
        return;
    }

    // Aktualizacja offsetu przewijania
    scrollOffset = std::max(0, std::min(static_cast<int>(moves.size() - maxVisibleMoves), scrollOffset + delta));

    // Aktualizacja pozycji scrollbara
    float scrollRatio = static_cast<float>(scrollOffset) / (moves.size() - maxVisibleMoves);
    float minY = scrollbar.getPosition().y;
    float maxY = scrollbar.getPosition().y + scrollbar.getSize().y - scrollbarThumb.getSize().y;
    float newY = minY + scrollRatio * (maxY - minY);

    scrollbarThumb.setPosition(scrollbarThumb.getPosition().x, newY);
}