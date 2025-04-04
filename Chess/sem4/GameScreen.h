#pragma once
#include "Screen.h"
#include "Button.h"
#include "ChessTimer.h"
// #include "GameInfoPanel.h"
#include "MoveHistoryPanel.h"
#include "ChessBoard.h"
#include "BoardView.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "ApplicationManager.h"

class GameScreen : public Screen {
private:
    // Komponenty UI
    sf::Font font;
    sf::RectangleShape background;
    Button backButton;
    Button resetButton;
    Button undoButton;

    // Komponenty gry
    ChessBoard chessBoard;        // Model planszy
    BoardView boardView;          // Widok planszy
    ChessTimer whiteTimer;        // Zegar dla bia³ego gracza
    ChessTimer blackTimer;        // Zegar dla czarnego gracza
    // GameInfoPanel infoPanel;      // Panel informacyjny
    MoveHistoryPanel historyPanel; // Panel historii ruchów

    // Stan gry
    bool isPieceSelected;         // Czy figura jest wybrana
    sf::Vector2i selectedPiecePos; // Pozycja wybranej figury
    bool gameOver;                // Czy gra siê zakoñczy³a
    bool currentPlayer;           // Aktualny gracz (true = bia³y, false = czarny)
    
    int whitePlayerTime;
    int blackPlayerTime;


    ApplicationManager* appManager;

    // Metody pomocnicze
    void handleBoardClick(const sf::Vector2i& mousePos);
    void makeMove(int fromRow, int fromCol, int toRow, int toCol);
    bool needsPromotion(int row, int col);
    std::string generateMoveNotation(int fromRow, int fromCol, int toRow, int toCol);
    void checkGameState();
    void undoLastMove();
    void updateBackgroundSize();

    bool showPopup;
    sf::RectangleShape popupBackground;
    sf::Text popupText;
    Button popupOkButton;
    std::string popupMessage;
    sf::Color popupColor;

    void showPopupWin(const std::string& message, sf::Color color);
    void hidePopup();
   

public:
    GameScreen(sf::RenderWindow& win, ApplicationManager* manager = nullptr);
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string handleEvent(const sf::Event& event) override;
    virtual void update() override;
    virtual void render() override;
    void resetGame();
    void setPlayerTimes(int whiteTime, int blackTime);


};