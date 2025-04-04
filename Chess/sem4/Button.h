#pragma once
#include <SFML/Graphics.hpp>
#include <string>
/**
 * Klasa Button reprezentuje interaktywny przycisk w interfejsie u¿ytkownika.
 * Pozwala na wyœwietlanie tekstu, reagowanie na najechanie mysz¹ i klikniêcia.
 */
class Button {
private:
    sf::RectangleShape shape;     // Kszta³t przycisku
    sf::Text text;                // Tekst na przycisku
    sf::Font font;                // Czcionka tekstu
    bool isHovered;               // Czy mysz znajduje siê nad przyciskiem
    sf::Color idleColor;          // Kolor przycisku w stanie spoczynku
    sf::Color hoverColor;         // Kolor przycisku po najechaniu mysz¹
public:
    /**
     * Konstruktor przycisku.
     * @param x Pozycja X przycisku
     * @param y Pozycja Y przycisku
     * @param width Szerokoœæ przycisku
     * @param height Wysokoœæ przycisku
     * @param buttonText Tekst wyœwietlany na przycisku
     * @param fontSize Rozmiar czcionki (domyœlnie 18)
     */
    Button(float x, float y, float width, float height, const std::string& buttonText, unsigned int fontSize = 18);
    /**
     * Ustawia czcionkê dla tekstu przycisku.
     * @param newFont Czcionka do ustawienia
     */
    void setFont(const sf::Font& newFont);
    /**
     * Ustawia pozycjê przycisku.
     * @param x Nowa pozycja X
     * @param y Nowa pozycja Y
     */
    void setPosition(float x, float y);
    /**
     * Ustawia kolory przycisku.
     * @param idle Kolor w stanie spoczynku
     * @param hover Kolor po najechaniu mysz¹
     */
    void setColors(const sf::Color& idle, const sf::Color& hover);
    /**
     * Ustawia kolor tekstu przycisku.
     * @param color Kolor tekstu do ustawienia
     */
    void setTextColor(const sf::Color& color);
    /**
     * Sprawdza czy punkt znajduje siê wewn¹trz przycisku.
     * @param point Punkt do sprawdzenia
     * @return true jeœli punkt jest wewn¹trz przycisku, false w przeciwnym razie
     */
    bool contains(const sf::Vector2f& point) const;
    /**
     * Sprawdza czy przycisk zosta³ klikniêty.
     * @param mousePos Pozycja klikniêcia myszy
     * @return true jeœli przycisk zosta³ klikniêty, false w przeciwnym razie
     */
    bool isClicked(const sf::Vector2i& mousePos) const;
    /**
     * Aktualizuje stan przycisku na podstawie pozycji myszy.
     * @param mousePos Aktualna pozycja myszy
     */
    void update(const sf::Vector2i& mousePos);
    /**
     * Renderuje przycisk w oknie.
     * @param window Okno, w którym ma zostaæ wyrenderowany przycisk
     */
    void render(sf::RenderWindow& window);
    void setText(const std::string& newText);

    void setTextStyle(sf::Uint32 style) {
        text.setStyle(style);
    }


};