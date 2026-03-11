#include "../include/HUD.h"

HUD::HUD(const sf::Font& font, int level, int score, int budget)
    : hudLevel(font), hudScore(font), hudBudget(font),
      playIcon(3), resetArrow1(3), resetArrow2(3)
{
    const float BTN_SIZE = 44.0f;
    const float BTN_Y = 4.0f;

    // ---- HUD texts ----
    hudLevel.setCharacterSize(18);
    hudLevel.setFillColor(sf::Color::White);
    hudLevel.setString("Level: " + std::to_string(level));
    hudLevel.setPosition({350.0f, 20.0f});

    hudScore.setCharacterSize(18);
    hudScore.setFillColor(sf::Color::White);
    hudScore.setString("Score: " + std::to_string(score));
    hudScore.setPosition({550.0f, 20.0f});

    hudBudget.setCharacterSize(18);
    hudBudget.setFillColor(sf::Color::White);
    hudBudget.setString("Budget: " + std::to_string(budget));
    hudBudget.setPosition({750.0f, 20.0f});

    // ---- Play/Pause ----
    playPauseBg.setSize(sf::Vector2f(BTN_SIZE, BTN_SIZE));
    playPauseBg.setPosition(sf::Vector2f(4.0f, BTN_Y));
    playPauseBg.setFillColor(sf::Color(120, 72, 30));
    playPauseBg.setOutlineColor(sf::Color(80, 48, 15));
    playPauseBg.setOutlineThickness(2.0f);

    float ppCx = 4.0f + BTN_SIZE / 2.0f;
    float ppCy = BTN_Y  + BTN_SIZE / 2.0f;

    playIcon.setPoint(0, sf::Vector2f(ppCx - 8.0f,  ppCy - 10.0f));
    playIcon.setPoint(1, sf::Vector2f(ppCx - 8.0f,  ppCy + 10.0f));
    playIcon.setPoint(2, sf::Vector2f(ppCx + 10.0f, ppCy));
    playIcon.setFillColor(sf::Color::White);

    pauseBar1.setSize(sf::Vector2f(9.0f, 20.0f));
    pauseBar2.setSize(sf::Vector2f(9.0f, 20.0f));
    pauseBar1.setPosition(sf::Vector2f(ppCx - 12.0f, ppCy - 10.0f));
    pauseBar2.setPosition(sf::Vector2f(ppCx +  3.0f, ppCy - 10.0f));
    pauseBar1.setFillColor(sf::Color::White);
    pauseBar2.setFillColor(sf::Color::White);

    // ---- Reset ----
    resetBg.setSize(sf::Vector2f(BTN_SIZE, BTN_SIZE));
    resetBg.setPosition(sf::Vector2f(52.0f, BTN_Y));
    resetBg.setFillColor(sf::Color(220, 185, 40));
    resetBg.setOutlineColor(sf::Color(0, 175, 185));
    resetBg.setOutlineThickness(3.0f);

    float rCx = 52.0f + BTN_SIZE / 2.0f;
    float rCy = BTN_Y  + BTN_SIZE / 2.0f;

    resetRing.setRadius(12.0f);
    resetRing.setOrigin(sf::Vector2f(12.0f, 12.0f));
    resetRing.setPosition(sf::Vector2f(rCx, rCy));
    resetRing.setFillColor(sf::Color::Transparent);
    resetRing.setOutlineColor(sf::Color::White);
    resetRing.setOutlineThickness(3.0f);

    resetArrow1.setPoint(0, sf::Vector2f(rCx - 3.0f, rCy - 12.0f));
    resetArrow1.setPoint(1, sf::Vector2f(rCx + 7.0f, rCy - 12.0f));
    resetArrow1.setPoint(2, sf::Vector2f(rCx + 2.0f, rCy -  6.0f));
    resetArrow1.setFillColor(sf::Color::White);

    resetArrow2.setPoint(0, sf::Vector2f(rCx + 3.0f, rCy + 12.0f));
    resetArrow2.setPoint(1, sf::Vector2f(rCx - 7.0f, rCy + 12.0f));
    resetArrow2.setPoint(2, sf::Vector2f(rCx - 2.0f, rCy +  6.0f));
    resetArrow2.setFillColor(sf::Color::White);

    // ---- Hamburger ----
    hamburgerBg.setSize(sf::Vector2f(BTN_SIZE, BTN_SIZE));
    hamburgerBg.setPosition(sf::Vector2f(1148.0f, BTN_Y));
    hamburgerBg.setFillColor(sf::Color(50, 170, 170));
    hamburgerBg.setOutlineColor(sf::Color(30, 130, 130));
    hamburgerBg.setOutlineThickness(2.0f);

    for (int i = 0; i < 3; i++) {
        hBars[i].setSize(sf::Vector2f(BTN_SIZE - 16.0f, 4.0f));
        hBars[i].setPosition(sf::Vector2f(1148.0f + 8.0f, BTN_Y + 11.0f + i * 9.0f));
        hBars[i].setFillColor(sf::Color::White);
    }

    // ---- Menu overlay ----
    menuOverlay.setSize(sf::Vector2f(1200.0f, 800.0f));
    menuOverlay.setFillColor(sf::Color(0, 0, 0, 180));

    menuPanel.setSize(sf::Vector2f(400.0f, 300.0f));
    menuPanel.setPosition(sf::Vector2f(400.0f, 250.0f));
    menuPanel.setFillColor(sf::Color(40, 40, 40, 230));
    menuPanel.setOutlineColor(sf::Color::White);
    menuPanel.setOutlineThickness(2.0f);
}

void HUD::update(int score, int budget) {
    hudScore.setString("Score: "  + std::to_string(score));
    hudBudget.setString("Budget: " + std::to_string(budget));
}

std::string HUD::handleClick(sf::Vector2f pos) {
    if (playPauseBg.getGlobalBounds().contains(pos))
        return "play";

    if (resetBg.getGlobalBounds().contains(pos))
        return "reset";

    if (hamburgerBg.getGlobalBounds().contains(pos)) {
        menuOpen = !menuOpen;
        return "menu";
    }

    // Click outside menu panel closes it
    if (menuOpen
        && !menuPanel.getGlobalBounds().contains(pos)
        && !hamburgerBg.getGlobalBounds().contains(pos)) {
        menuOpen = false;
    }

    return "";
}

void HUD::handleKeyEscape() {
    menuOpen = false;
}

void HUD::draw(sf::RenderWindow& window, bool simRunning) const {
    window.draw(hudLevel);
    window.draw(hudScore);
    window.draw(hudBudget);

    // Play/Pause
    window.draw(playPauseBg);
    if (simRunning) {
        window.draw(pauseBar1);
        window.draw(pauseBar2);
    } else {
        window.draw(playIcon);
    }

    // Reset
    window.draw(resetBg);
    window.draw(resetRing);
    window.draw(resetArrow1);
    window.draw(resetArrow2);

    // Hamburger
    window.draw(hamburgerBg);
    for (const auto& bar : hBars)
        window.draw(bar);

    // Menu overlay (on top)
    if (menuOpen) {
        window.draw(menuOverlay);
        window.draw(menuPanel);
    }
}
