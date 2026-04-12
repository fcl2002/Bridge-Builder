
#include <iostream>
#include "include/HUD.h"
#include "include/Level.h"
#include <SFML/Graphics.hpp>

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
    int budget = getInitialBudgetForLevel(currentLevel);
    int score = 0;

    while (window.isOpen()) {
        HUD hud(font, currentLevel, score, budget);

        std::unique_ptr<Level> level;
        if (currentLevel == 1) {
            level = std::make_unique<Level1>();
        } else if (currentLevel == 2) {
            level = std::make_unique<Level2>();
        } else if (currentLevel == 3) {
            level = std::make_unique<Level3>();
        }

        if (!level) {
            std::cout << "[ERROR] Invalid level selected: " << currentLevel << "\n";
            break;
        }

        const int previousLevel = currentLevel;
        level->run(window, hud, score, budget, simRunning, currentLevel);

        if (!window.isOpen()) {
            break;
        }

        if (currentLevel != previousLevel) {
            simRunning = false;
            score = 0;
            budget = getInitialBudgetForLevel(currentLevel);
        }
    }

    return 0;
}
