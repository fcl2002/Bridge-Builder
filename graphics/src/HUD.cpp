#include "../include/HUD.h"
#include <SFML/Graphics/Text.hpp>

#include <initializer_list>

namespace {
bool loadTextureFromAny(sf::Texture& texture, std::initializer_list<const char*> paths) {
    for (const char* path : paths) {
        if (texture.loadFromFile(path)) {
            return true;
        }
    }
    return false;
}
}

HUD::HUD(const sf::Font& font, int level, int score, int budget)
    : hudLevel(font), hudScore(font), hudBudget(font), playIcon(3)
{
    const float BTN_SIZE = 44.0f;
    const float BTN_Y = 8.0f;

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
    // Unified button style
    const float BTN_X = 16.0f;
    const float BTN_SPACING = 16.0f; // Increased spacing
    playPauseBg.setSize(sf::Vector2f(BTN_SIZE, BTN_SIZE));
    playPauseBg.setPosition(sf::Vector2f(BTN_X, BTN_Y));
    playPauseBg.setFillColor(sf::Color(38, 166, 154)); // #26A69A
    playPauseBg.setOutlineColor(sf::Color(0, 77, 64)); // #004D40
    playPauseBg.setOutlineThickness(2.0f);

    float ppCx = BTN_X + BTN_SIZE / 2.0f;
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

    // Reset button with image
    resetBg.setSize(sf::Vector2f(BTN_SIZE, BTN_SIZE));
    resetBg.setPosition(sf::Vector2f(BTN_X + BTN_SIZE + BTN_SPACING, BTN_Y));
    resetBg.setFillColor(sf::Color(38, 166, 154)); // #26A69A
    resetBg.setOutlineColor(sf::Color(0, 77, 64)); // #004D40
    resetBg.setOutlineThickness(2.0f);

    // Load refresh icon
    if (loadTextureFromAny(resetTexture, {
        "assets/icons/refresh-icon.png",
        "graphics/assets/icons/refresh-icon.png",
        "assets/icons/refresh-icon.png",
        "graphics/assets/icons/refresh-icon.png"
    })) {
        resetSprite = std::make_unique<sf::Sprite>(resetTexture);
        // Padding in pixels
        const float padding = 6.0f;
        float iconTargetSize = BTN_SIZE - 2 * padding;
        float scaleX = iconTargetSize / static_cast<float>(resetTexture.getSize().x);
        float scaleY = iconTargetSize / static_cast<float>(resetTexture.getSize().y);
        resetSprite->setScale(sf::Vector2f(scaleX, scaleY));
        // Center the icon inside the button
        sf::Vector2f pos = resetBg.getPosition();
        pos.x += padding;
        pos.y += padding;
        resetSprite->setPosition(pos);
    }

    // ---- Hamburger ----
    hamburgerBg.setSize(sf::Vector2f(BTN_SIZE, BTN_SIZE));
    hamburgerBg.setPosition(sf::Vector2f(1140.0f, BTN_Y));
    hamburgerBg.setFillColor(sf::Color(38, 166, 154)); // #26A69A
    hamburgerBg.setOutlineColor(sf::Color(0, 77, 64)); // #004D40
    hamburgerBg.setOutlineThickness(2.0f);

    for (int i = 0; i < 3; i++) {
        hBars[i].setSize(sf::Vector2f(BTN_SIZE - 16.0f, 4.0f));
        hBars[i].setPosition(sf::Vector2f(1140.0f + 8.0f, BTN_Y + 11.0f + i * 9.0f));
        hBars[i].setFillColor(sf::Color::White);
    }

    // ---- Wood tool ----
    woodBg.setSize(sf::Vector2f(BTN_SIZE, BTN_SIZE));
    woodBg.setPosition(sf::Vector2f(1140.0f, 200.0f));
    woodBg.setFillColor(sf::Color(38, 166, 154)); // #26A69A
    woodBg.setOutlineColor(sf::Color(0, 77, 64)); // #004D40
    woodBg.setOutlineThickness(2.0f);

    // Load wood icon
    if (loadTextureFromAny(woodTexture, {
        "assets/icons/wood.png",
        "graphics/assets/icons/wood.png",
        "assets/icons/wood.png",
        "graphics/assets/icons/wood.png"
    })) {
        woodSprite = std::make_unique<sf::Sprite>(woodTexture);
        // Padding for the icon
        const float padding = 8.0f;
        float iconTargetWidth = woodBg.getSize().x - 2 * padding;
        float iconTargetHeight = woodBg.getSize().y - 2 * padding;
        float scaleX = iconTargetWidth / static_cast<float>(woodTexture.getSize().x);
        float scaleY = iconTargetHeight / static_cast<float>(woodTexture.getSize().y);
        woodSprite->setScale(sf::Vector2f(scaleX, scaleY));
        // Center the icon inside the button
        sf::Vector2f pos = woodBg.getPosition();
        pos.x += padding;
        pos.y += padding;
        woodSprite->setPosition(pos);
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
    if (playPauseBg.getGlobalBounds().contains(pos)) {
        clearToolSelection();
        return "play";
    }

    if (resetBg.getGlobalBounds().contains(pos)) {
        clearToolSelection();
        return "reset";
    }

    if (woodBg.getGlobalBounds().contains(pos)) {
        woodModeActive = !woodModeActive;
        return woodModeActive ? "wood_on" : "wood_off";
    }

    if (hamburgerBg.getGlobalBounds().contains(pos)) {
        clearToolSelection();
        menuOpen = !menuOpen;
        return "menu";
    }

    if (menuOpen && menuPanel.getGlobalBounds().contains(pos)) {
        return "menu";
    }

    // Click outside menu panel closes it
    if (menuOpen
        && !menuPanel.getGlobalBounds().contains(pos)
        && !hamburgerBg.getGlobalBounds().contains(pos)) {
        menuOpen = false;
        return "menu";
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
    if (resetSprite) {
        window.draw(*resetSprite);
    }

    // Wood tool
    window.draw(woodBg);
    if (woodSprite) {
        window.draw(*woodSprite);
    }
    // Tooltip on hover
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    if (woodBg.getGlobalBounds().contains(mousePosF)) {
        sf::Text tooltip(hudLevel.getFont(), "Wood", 12);
        tooltip.setFillColor(sf::Color::White);
        tooltip.setOutlineColor(sf::Color(0,0,0));
        tooltip.setOutlineThickness(2.0f);
        const float spacing = 8.0f;
        const sf::FloatRect bounds = tooltip.getLocalBounds();
        const sf::Vector2f woodPos = woodBg.getPosition();
        const float tooltipX = woodPos.x - bounds.size.x - spacing - bounds.position.x;
        const float tooltipY = woodPos.y + (woodBg.getSize().y - bounds.size.y) / 2.0f - bounds.position.y;
        tooltip.setPosition(sf::Vector2f(tooltipX, tooltipY));
        window.draw(tooltip);
    }

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

bool HUD::isPointInHUD(sf::Vector2f pos) const {
    if (menuOpen) {
        return true;
    }

    return playPauseBg.getGlobalBounds().contains(pos)
        || resetBg.getGlobalBounds().contains(pos)
        || woodBg.getGlobalBounds().contains(pos)
        || hamburgerBg.getGlobalBounds().contains(pos);
}

void HUD::clearToolSelection() {
    woodModeActive = false;
}
