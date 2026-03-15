
#include <SFML/Graphics.hpp>
#include "include/HUD.h"
#include "include/Level.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Bridge Builder - Game");

    sf::Font font;
    const bool loadedFromGraphicsCwd = font.openFromFile("assets/Press_Start_2P/PressStart2P-Regular.ttf");
    const bool loadedFromRepoRootCwd = loadedFromGraphicsCwd
        ? true
        : font.openFromFile("graphics/assets/Press_Start_2P/PressStart2P-Regular.ttf");
    if (!loadedFromRepoRootCwd)
        return -1;

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
