#pragma once

#include <SFML/Graphics.hpp>
#include "Board.h"

class Graph {
protected:
    sf::RenderWindow window;
    sf::Texture boardTexture;
    sf::Sprite boardSprite;
    sf::Texture piecesTextures[12]; // Tablica tekstur dla pionk�w
    sf::Sprite piecesSprites[8][8]; // Tablica sprite'�w dla pionk�w na planszy
    ChessBoard chessBoard;
   

public:
    // Konstruktor
    Graph() {
        // Utw�rz okno o okre�lonych wymiarach i tytule
        window.create(sf::VideoMode(600, 600), "Chess Game");

        // Wczytaj tekstur� planszy
        if (!boardTexture.loadFromFile("graphs/board.png")) {
            // Obs�uga b��du �adowania tekstury planszy
            std::cerr << "Failed to load board texture!" << std::endl;
        }

        // Ustaw tekstur� planszy na sprite
        boardSprite.setTexture(boardTexture);

        // Wczytaj tekstury dla pionk�w
        std::string piecesPaths[12] = {
            "graphs/rook_b.png",   // -1 - wie�a czarna
            "graphs/knight_b.png", // -2 - skoczek czarny
            "graphs/bishop_b.png", // -3 - goniec czarny
            "graphs/king_b.png",  // -4 - kr�lowa czarna
            "graphs/queen_b.png",   // -5 - kr�l czarny
            "graphs/pawn_b.png",   // -6 - pionek czarny
            "graphs/rook_w.png",   //  1 - wie�a bia�a
            "graphs/knight_w.png", //  2 - skoczek bia�y
            "graphs/bishop_w.png", //  3 - goniec bia�y
            "graphs/king_w.png",  //  4 - kr�lowa bia�a
            "graphs/queen_w.png",   //  5 - kr�l bia�y
            "graphs/pawn_w.png"    //  6 - pionek bia�y
        };

        for (int i = 0; i < 12; ++i) {
            if (!piecesTextures[i].loadFromFile(piecesPaths[i])) {
                // Obs�uga b��du �adowania tekstur pionk�w
                std::cerr << "Failed to load piece texture: " << piecesPaths[i] << std::endl;
            }
        }

        // Ustawienie sprite'�w dla pionk�w na planszy
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                piecesSprites[i][j].setPosition(j * 75, i * 75);
            }
        }
    }

 
    // Metoda renderuj�ca plansz� i warto�ci z tablicy
    void render() {
        // Wyczy�� okno przed renderowaniem
        window.clear();

        // Renderuj plansz�
        window.draw(boardSprite);

        // Wy�wietl warto�ci z tablicy planszy
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                int piece = chessBoard.getPieceAt(i, j);
                if (piece != 0) {
                    if (piece > 0) // Bia�e pionki
                        piecesSprites[i][j].setTexture(piecesTextures[piece + 5]);
                    else // Czarne pionki
                        piecesSprites[i][j].setTexture(piecesTextures[abs(piece) - 1]);
                    window.draw(piecesSprites[i][j]);
                }
            }
        }

        // Wy�wietl renderowan� zawarto�� na ekranie
        window.display();
    }

    // Metoda zwracaj�ca referencj� do okna
    sf::RenderWindow& getWindow() {
        return window;
    }

    // Metoda zwracaj�ca referencj� do obiektu ChessBoard
    ChessBoard& getChessBoard() {
        return chessBoard;
    }

    // Metoda zwracaj�ca wska�nik do tablicy planszy
    const int(*getBoard() const)[8] {
        return chessBoard.getBoard();
        }

    // Metoda zwracaj�ca warto�� na planszy dla danego pola
            int getPieceAt(int row, int col) const {
            return chessBoard.getPieceAt(row, col);
        }
};
