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
    // Returns "play", "reset", "menu" or "" depending on what was clicked
    std::string handleClick(sf::Vector2f pos);

    void handleKeyEscape();

    void draw(sf::RenderWindow& window, bool simRunning) const;

    bool isMenuOpen() const { return menuOpen; }

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
    sf::CircleShape resetRing;
    sf::ConvexShape resetArrow1;
    sf::ConvexShape resetArrow2;

    // Hamburger button
    sf::RectangleShape hamburgerBg;
    std::array<sf::RectangleShape, 3> hBars;

    // Menu overlay
    sf::RectangleShape menuOverlay;
    sf::RectangleShape menuPanel;

    bool menuOpen = false;
};
