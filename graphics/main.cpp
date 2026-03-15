
#include <SFML/Graphics.hpp>
#include "include/HUD.h"
#include "include/Level.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Bridge Builder - Game");

    sf::Font font;
    bool loadedFont = false;
    if (font.openFromFile("assets/Press_Start_2P/PressStart2P-Regular.ttf")) {
        std::cout << "[OK] Loaded font: assets/Press_Start_2P/PressStart2P-Regular.ttf\n";
        loadedFont = true;
    } else if (font.openFromFile("graphics/assets/Press_Start_2P/PressStart2P-Regular.ttf")) {
        std::cout << "[OK] Loaded font: graphics/assets/Press_Start_2P/PressStart2P-Regular.ttf\n";
        loadedFont = true;
    } else {
        std::cout << "[ERROR] Failed to load font: assets/Press_Start_2P/PressStart2P-Regular.ttf or graphics/assets/Press_Start_2P/PressStart2P-Regular.ttf\n";
    }
    if (!loadedFont) return -1;

    // Game state variables
    bool simRunning = false;
    int currentLevel = 1;
    int budget = 1000;
    int score = 0;

    HUD hud(font, currentLevel, score, budget);

    // Modular: instantiate level 1 only when currentLevel == 1.
    std::unique_ptr<Level> level;
    if (currentLevel == 1) {
        level = std::make_unique<Level1>();
    }

    if (level) {
        level->run(window, hud, score, budget, simRunning);
    }

    return 0;
}
