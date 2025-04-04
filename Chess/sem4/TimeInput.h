#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class TimeInputField {
private:
    // Position and size
    sf::Vector2f position;
    sf::Vector2f size;

    // Minutes and seconds
    int minutes;
    int seconds;

    // Min and max values
    int minMinutes;
    int maxMinutes;
    int minSeconds;
    int maxSeconds;

    // Visual elements
    sf::RectangleShape background;
    sf::RectangleShape minutesBox;
    sf::RectangleShape secondsBox;

    // Arrows for minutes and seconds (changed to ConvexShape for triangles)
    sf::ConvexShape minutesUpArrow;
    sf::ConvexShape minutesDownArrow;
    sf::ConvexShape secondsUpArrow;
    sf::ConvexShape secondsDownArrow;

    // Text
    sf::Font font;
    sf::Text minutesText;
    sf::Text secondsText;
    sf::Text separatorText;
    sf::Text labelText;

    // Active element (minutes or seconds)
    enum class ActivePart { None, Minutes, Seconds };
    ActivePart activePart;

    // Callback function when value changes
    std::function<void(int, int)> onChange;

public:
    TimeInputField(const sf::Vector2f& pos, const sf::Vector2f& sz, const std::string& label = "Time");

    void render(sf::RenderWindow& window);
    bool handleEvent(const sf::Event& event, const sf::Vector2i& mousePos);
    void update(const sf::Vector2i& mousePos);

    // Set callback for value change
    void setOnChange(std::function<void(int, int)> callback);

    // Get total time in seconds
    int getTotalSeconds() const;

    // Set time
    void setTime(int mins, int secs);

    // Get minutes and seconds
    int getMinutes() const;
    int getSeconds() const;

private:
    void updateMinutesText();
    void updateSecondsText();
    std::string formatNumber(int number);
    void centerTextInBox(sf::Text& text, const sf::RectangleShape& box);
    void incrementMinutes();
    void decrementMinutes();                
    void incrementSeconds();
    void decrementSeconds();
    void notifyChange();
};