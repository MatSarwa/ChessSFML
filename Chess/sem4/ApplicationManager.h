#pragma once
#include <map>
#include <string>
#include <SFML/Audio.hpp>
#include "Screen.h"


class Button;
class MenuScreen;
class OptionsScreen;
class GameScreen;

class ApplicationManager {
private:
    sf::RenderWindow window;
    std::map<std::string, Screen*> screens;
    Screen* currentScreen;
    sf::Music backgroundMusic;
    bool isMusicEnabled;
    float musicVolume;
    OptionsScreen* optionsScreenPtr;

    int whitePlayerTimeSeconds = 900; 
    int blackPlayerTimeSeconds = 900; 


public:

    ApplicationManager() : currentScreen(nullptr), isMusicEnabled(true), musicVolume(50.0f) {}
    ~ApplicationManager() {
        // Zwolnienie zasobów
        for (auto& pair : screens) {
            delete pair.second;
        }
    }

    OptionsScreen* getOptionsScreen() { return optionsScreenPtr; }


    void setWhitePlayerTime(int seconds) { whitePlayerTimeSeconds = seconds; }
    void setBlackPlayerTime(int seconds) { blackPlayerTimeSeconds = seconds; }
    int getWhitePlayerTime() const { return whitePlayerTimeSeconds; }
    int getBlackPlayerTime() const { return blackPlayerTimeSeconds; }
 


    void setPlayerTimes(int whiteTimeSeconds, int blackTimeSeconds);
    void initialize();
    void setScreen(const std::string& screenName);
    void toggleMusic(bool enable);
    void setMusicVolume(float volume);
    bool getMusicEnabled() const;
    void run();
};