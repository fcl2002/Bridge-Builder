#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <string>

class HUD {
public:
    HUD(const sf::Font& font, int level, int score, int budget);

    // Call once per frame with updated game state
    void update(int score, int budget);

    // Handle a left-click at window position pos
    // Returns "play", "reset", "menu", "wood_on", "wood_off" or ""
    std::string handleClick(sf::Vector2f pos);

    void handleKeyEscape();

    void draw(sf::RenderWindow& window, bool simRunning) const;

    bool isMenuOpen() const { return menuOpen; }
    bool isWoodModeActive() const { return woodModeActive; }
    bool isPointInHUD(sf::Vector2f pos) const;
    void clearToolSelection();

private:
    // HUD texts
    sf::Text hudLevel;
    sf::Text hudScore;
    sf::Text hudBudget;

    // Play/Pause button
    sf::RectangleShape playPauseBg;
    sf::ConvexShape    playIcon;
    sf::RectangleShape pauseBar1;
    sf::RectangleShape pauseBar2;

    // Reset button
    sf::RectangleShape resetBg;
    sf::Texture resetTexture;
    std::unique_ptr<sf::Sprite> resetSprite;

    // Hamburger button
    sf::RectangleShape hamburgerBg;
    std::array<sf::RectangleShape, 3> hBars;

    // Wood construction button
    sf::RectangleShape woodBg;
    sf::Texture woodTexture;
    std::unique_ptr<sf::Sprite> woodSprite;

    bool woodModeActive = false;

    // Menu overlay
    sf::RectangleShape menuOverlay;
    sf::RectangleShape menuPanel;
    sf::RectangleShape level1Btn;
    sf::RectangleShape level2Btn;
    sf::Text level1Text;
    sf::Text level2Text;
    sf::RectangleShape level3Btn;
    sf::Text level3Text;

    bool menuOpen = false;
};
