#pragma once

#include <SFML/Graphics.hpp>
#include "Board.h"

class Graph {
protected:
    sf::RenderWindow window;
    sf::Texture boardTexture;
    sf::Sprite boardSprite;
    sf::Texture piecesTextures[12]; // Tablica tekstur dla pionków
    sf::Sprite piecesSprites[8][8]; // Tablica sprite'ów dla pionków na planszy
    ChessBoard chessBoard;
   

public:
    // Konstruktor
    Graph() {
        // Utwórz okno o okreœlonych wymiarach i tytule
        window.create(sf::VideoMode(600, 600), "Chess Game");

        // Wczytaj teksturê planszy
        if (!boardTexture.loadFromFile("graphs/board.png")) {
            // Obs³uga b³êdu ³adowania tekstury planszy
            std::cerr << "Failed to load board texture!" << std::endl;
        }

        // Ustaw teksturê planszy na sprite
        boardSprite.setTexture(boardTexture);

        // Wczytaj tekstury dla pionków
        std::string piecesPaths[12] = {
            "graphs/rook_b.png",   // -1 - wie¿a czarna
            "graphs/knight_b.png", // -2 - skoczek czarny
            "graphs/bishop_b.png", // -3 - goniec czarny
            "graphs/king_b.png",  // -4 - królowa czarna
            "graphs/queen_b.png",   // -5 - król czarny
            "graphs/pawn_b.png",   // -6 - pionek czarny
            "graphs/rook_w.png",   //  1 - wie¿a bia³a
            "graphs/knight_w.png", //  2 - skoczek bia³y
            "graphs/bishop_w.png", //  3 - goniec bia³y
            "graphs/king_w.png",  //  4 - królowa bia³a
            "graphs/queen_w.png",   //  5 - król bia³y
            "graphs/pawn_w.png"    //  6 - pionek bia³y
        };

        for (int i = 0; i < 12; ++i) {
            if (!piecesTextures[i].loadFromFile(piecesPaths[i])) {
                // Obs³uga b³êdu ³adowania tekstur pionków
                std::cerr << "Failed to load piece texture: " << piecesPaths[i] << std::endl;
            }
        }

        // Ustawienie sprite'ów dla pionków na planszy
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                piecesSprites[i][j].setPosition(j * 75, i * 75);
            }
        }
    }

 
    // Metoda renderuj¹ca planszê i wartoœci z tablicy
    void render() {
        // Wyczyœæ okno przed renderowaniem
        window.clear();

        // Renderuj planszê
        window.draw(boardSprite);

        // Wyœwietl wartoœci z tablicy planszy
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                int piece = chessBoard.getPieceAt(i, j);
                if (piece != 0) {
                    if (piece > 0) // Bia³e pionki
                        piecesSprites[i][j].setTexture(piecesTextures[piece + 5]);
                    else // Czarne pionki
                        piecesSprites[i][j].setTexture(piecesTextures[abs(piece) - 1]);
                    window.draw(piecesSprites[i][j]);
                }
            }
        }

        // Wyœwietl renderowan¹ zawartoœæ na ekranie
        window.display();
    }

    // Metoda zwracaj¹ca referencjê do okna
    sf::RenderWindow& getWindow() {
        return window;
    }

    // Metoda zwracaj¹ca referencjê do obiektu ChessBoard
    ChessBoard& getChessBoard() {
        return chessBoard;
    }

    // Metoda zwracaj¹ca wskaŸnik do tablicy planszy
    const int(*getBoard() const)[8] {
        return chessBoard.getBoard();
        }

    // Metoda zwracaj¹ca wartoœæ na planszy dla danego pola
            int getPieceAt(int row, int col) const {
            return chessBoard.getPieceAt(row, col);
        }
};
