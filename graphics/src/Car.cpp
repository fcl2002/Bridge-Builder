#include "../include/Car.h"

// Pixel size: each "cell" in the pixel art grid = P x P screen pixels
static constexpr float P = 8.0f;

// Colour palette  (sampled from the reference image)
static const sf::Color DARK_RED   (110,  30,  50);
static const sf::Color MID_RED    (150,  40,  60);
static const sf::Color LIGHT_RED  (185,  60,  80);
static const sf::Color ORANGE_ACC (200, 100,  50);
static const sf::Color WINDOW_BLU (100, 185, 210);
static const sf::Color HEADLIGHT  (220, 190,  60);
static const sf::Color WHEEL_DARK ( 30,  30,  30);
static const sf::Color WHEEL_RIM  ( 90,  90,  90);
static const sf::Color BLACK      (  0,   0,   0);

Car::Car(sf::Vector2f pos) {
    // pos is the bottom-left of the car body row.
    // Grid origin: top-left of the sprite = pos - (0, HEIGHT)
    // Sprite is 12 cells wide × 8 cells tall = 96×64 px

    // We define the grid top-left so that the wheels bottom sits at pos.y
    sf::Vector2f o(pos.x, pos.y - 8 * P);

    // ---- Body outline / underbody (row 5-6, full width minus wheels) ----
    addBlock(o,  1, 5, 10, 2, MID_RED);

    // ---- Main body upper section (row 3-4) ----
    addBlock(o,  1, 3, 10, 2, MID_RED);

    // ---- Roof / cabin arch (row 1-2, cols 3-9) ----
    addBlock(o,  3, 1,  6, 2, DARK_RED);
    // Roof highlight
    addBlock(o,  4, 1,  4, 1, LIGHT_RED);

    // ---- Windshield front (row 2-3, cols 3-5) ----
    addBlock(o,  3, 2,  2, 2, WINDOW_BLU);

    // ---- Rear window (row 2-3, cols 7-9) ----
    addBlock(o,  7, 2,  2, 2, WINDOW_BLU);

    // ---- Orange accent strip (row 4, full body) ----
    addBlock(o,  1, 4, 10, 1, ORANGE_ACC);

    // ---- Headlights (row 4-5, col 1) ----
    addBlock(o,  1, 4,  1, 2, LIGHT_RED);

    // ---- Rear lights (row 4-5, col 10) ----
    addBlock(o, 10, 4,  1, 2, HEADLIGHT);

    // ---- Black outline bottom ----
    addBlock(o,  1, 6, 10, 1, BLACK);

    // ---- Front bumper col 0 ----
    addBlock(o,  0, 5,  1, 2, DARK_RED);

    // ---- Rear bumper col 11 ----
    addBlock(o, 11, 5,  1, 2, DARK_RED);

    // ---- Wheels: two big circles approximated as dark squares with rim ----
    // Front wheel  (cols 2-3, rows 6-7)
    addBlock(o,  2, 6,  2, 2, WHEEL_DARK);
    addBlock(o,  2, 6,  2, 2, WHEEL_DARK);  // center
    addBlock(o,  2, 6,  1, 1, WHEEL_RIM);   // rim highlight

    // Rear wheel  (cols 8-9, rows 6-7)
    addBlock(o,  8, 6,  2, 2, WHEEL_DARK);
    addBlock(o,  9, 6,  1, 1, WHEEL_RIM);   // rim highlight

    // ---- Ground shadow ---- (thin dark strip, row 7-ish)
    addBlock(o,  2, 7,  2, 1, sf::Color(0, 0, 0, 80));
    addBlock(o,  8, 7,  2, 1, sf::Color(0, 0, 0, 80));
}

void Car::addBlock(sf::Vector2f base, int col, int row, int w, int h, sf::Color color) {
    sf::RectangleShape rect(sf::Vector2f(w * P, h * P));
    rect.setPosition(sf::Vector2f(base.x + col * P, base.y + row * P));
    rect.setFillColor(color);
    blocks.push_back(rect);
}

void Car::move(float dx) {
    for (auto& b : blocks)
        b.move(sf::Vector2f(dx, 0.0f));
}

void Car::draw(sf::RenderWindow& window) const {
    for (const auto& b : blocks)
        window.draw(b);
}
