    #pragma once
    #include <SFML/Graphics.hpp>
    #include <vector>
    #include <string>

    class GameInfoPanel {
    private:
        sf::RenderWindow* window;
        sf::Font font;

        // Pozycja i wymiary panelu
        sf::Vector2f position;
        sf::Vector2f size;

        // Elementy panelu
        sf::RectangleShape background;
        sf::Text titleText;
        sf::Text currentPlayerText;
        sf::Text gameStateText;

        // Kontenery na zbite figury
        struct CapturedPiece {
            sf::Sprite sprite;
            int value;  // Wartoœæ figury (przydatne do sortowania)
        };

        std::vector<CapturedPiece> whiteCaptured;  // Figury zbite przez bia³ego
        std::vector<CapturedPiece> blackCaptured;  // Figury zbite przez czarnego

        // Tekstury figur
        std::map<int, sf::Texture> pieceTextures;

    public:
        GameInfoPanel(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& size);

        void render();
        void setCurrentPlayer(bool isWhite);
        void setGameState(const std::string& state);

        void addCapturedPiece(int pieceValue);
        void clearCapturedPieces();

        // Obliczanie ró¿nicy materia³u
        int getMaterialDifference() const;
    };