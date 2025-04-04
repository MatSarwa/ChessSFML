#pragma once
#include <SFML/Graphics.hpp>
#include <string>
/**
 * Klasa Button reprezentuje interaktywny przycisk w interfejsie u�ytkownika.
 * Pozwala na wy�wietlanie tekstu, reagowanie na najechanie mysz� i klikni�cia.
 */
class Button {
private:
    sf::RectangleShape shape;     // Kszta�t przycisku
    sf::Text text;                // Tekst na przycisku
    sf::Font font;                // Czcionka tekstu
    bool isHovered;               // Czy mysz znajduje si� nad przyciskiem
    sf::Color idleColor;          // Kolor przycisku w stanie spoczynku
    sf::Color hoverColor;         // Kolor przycisku po najechaniu mysz�
public:
    /**
     * Konstruktor przycisku.
     * @param x Pozycja X przycisku
     * @param y Pozycja Y przycisku
     * @param width Szeroko�� przycisku
     * @param height Wysoko�� przycisku
     * @param buttonText Tekst wy�wietlany na przycisku
     * @param fontSize Rozmiar czcionki (domy�lnie 18)
     */
    Button(float x, float y, float width, float height, const std::string& buttonText, unsigned int fontSize = 18);
    /**
     * Ustawia czcionk� dla tekstu przycisku.
     * @param newFont Czcionka do ustawienia
     */
    void setFont(const sf::Font& newFont);
    /**
     * Ustawia pozycj� przycisku.
     * @param x Nowa pozycja X
     * @param y Nowa pozycja Y
     */
    void setPosition(float x, float y);
    /**
     * Ustawia kolory przycisku.
     * @param idle Kolor w stanie spoczynku
     * @param hover Kolor po najechaniu mysz�
     */
    void setColors(const sf::Color& idle, const sf::Color& hover);
    /**
     * Ustawia kolor tekstu przycisku.
     * @param color Kolor tekstu do ustawienia
     */
    void setTextColor(const sf::Color& color);
    /**
     * Sprawdza czy punkt znajduje si� wewn�trz przycisku.
     * @param point Punkt do sprawdzenia
     * @return true je�li punkt jest wewn�trz przycisku, false w przeciwnym razie
     */
    bool contains(const sf::Vector2f& point) const;
    /**
     * Sprawdza czy przycisk zosta� klikni�ty.
     * @param mousePos Pozycja klikni�cia myszy
     * @return true je�li przycisk zosta� klikni�ty, false w przeciwnym razie
     */
    bool isClicked(const sf::Vector2i& mousePos) const;
    /**
     * Aktualizuje stan przycisku na podstawie pozycji myszy.
     * @param mousePos Aktualna pozycja myszy
     */
    void update(const sf::Vector2i& mousePos);
    /**
     * Renderuje przycisk w oknie.
     * @param window Okno, w kt�rym ma zosta� wyrenderowany przycisk
     */
    void render(sf::RenderWindow& window);
    void setText(const std::string& newText);

    void setTextStyle(sf::Uint32 style) {
        text.setStyle(style);
    }


};