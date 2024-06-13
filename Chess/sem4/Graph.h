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



        void promotePawn(sf::RenderWindow& window, int row, int col, bool isWhite) {
            // Rozmiar pola na planszy
            int squareSize = window.getSize().x / 8;

            // Stworzenie opcji promocji
            std::vector<sf::Sprite> promotionOptions;
            std::vector<int> promotionValues;
            sf::Texture queenTexture, knightTexture, bishopTexture, rookTexture;

            if (isWhite) {
                // Promocja dla bia�ych
                promotionValues = { 5, 3, 2, 1 }; // Queen, Knight, Bishop, Rook
                if (!queenTexture.loadFromFile("graphs/queen_w.png") ||
                    !knightTexture.loadFromFile("graphs/knight_w.png") ||
                    !bishopTexture.loadFromFile("graphs/bishop_w.png") ||
                    !rookTexture.loadFromFile("graphs/rook_w.png")) {
                    std::cerr << "Failed to load piece textures!" << std::endl;
                    return;
                }
            }
            else {
                // Promocja dla czarnych
                promotionValues = { -5, -3, -2, -1 }; // Queen, Knight, Bishop, Rook
                if (!queenTexture.loadFromFile("graphs/queen_b.png") ||
                    !knightTexture.loadFromFile("graphs/knight_b.png") ||
                    !bishopTexture.loadFromFile("graphs/bishop_b.png") ||
                    !rookTexture.loadFromFile("graphs/rook_b.png")) {
                    std::cerr << "Failed to load piece textures!" << std::endl;
                    return;
                }
            }

            // Ustawienie grafik pionk�w na pozycje wyboru promocji
            sf::Sprite queenSprite(queenTexture);
            sf::Sprite knightSprite(knightTexture);
            sf::Sprite bishopSprite(bishopTexture);
            sf::Sprite rookSprite(rookTexture);

            queenSprite.setPosition(col * squareSize, (row - 1) * squareSize);
            knightSprite.setPosition(col * squareSize, (row - 2) * squareSize);
            bishopSprite.setPosition(col * squareSize, (row - 3) * squareSize);
            rookSprite.setPosition(col * squareSize, (row - 4) * squareSize);

            promotionOptions.push_back(queenSprite);
            promotionOptions.push_back(knightSprite);
            promotionOptions.push_back(bishopSprite);
            promotionOptions.push_back(rookSprite);

            // Utworzenie bufora renderowania
            sf::RenderTexture renderTexture;
            renderTexture.create(window.getSize().x, window.getSize().y);
            renderTexture.clear();

            // Renderowanie planszy
            render();
            renderTexture.display();
            sf::Sprite boardSprite(renderTexture.getTexture());
            window.draw(boardSprite);

            // Renderowanie opcji promocji
            for (const auto& option : promotionOptions) {
                renderTexture.draw(option);
            }
            renderTexture.display();
            sf::Sprite promotionSprite(renderTexture.getTexture());
            window.draw(promotionSprite);

            window.display();

            // Oczekiwanie na wyb�r u�ytkownika
            bool promotionChosen = false;
            while (!promotionChosen) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                        for (int i = 0; i < promotionOptions.size(); ++i) {
                            if (promotionOptions[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                                // U�ytkownik wybra� opcj� promocji
                                int promotionValue = promotionValues[i];
                                // Aktualizacja planszy
                                chessBoard.setPieceAt(row, col, promotionValue);
                                promotionChosen = true;
                                break;
                            }
                        }
                    }
                }
            }

            // Renderowanie planszy po dokonaniu promocji
            window.clear();
            render();
            window.display();
        }





};