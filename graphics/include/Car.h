#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Car {
public:
    // pos: bottom-left anchor position of the car on screen
    explicit Car(sf::Vector2f pos);

    // Move the car by a delta vector (dx, dy) in pixels (added in cm)
    void move(const sf::Vector2f& delta);

    // Convenience helper for horizontal-only movement (legacy)
    void move(float dx);

    void draw(sf::RenderWindow& window) const;

    // Returns the bounding width of the car
    float getWidth()  const { return 96.0f; }
    float getHeight() const { return 64.0f; }

private:
    // Each "pixel block" in the pixel-art is a small rectangle
    std::vector<sf::RectangleShape> blocks;

    // Helper: add a pixel block at grid cell (col, row) with given colour.
    // Each cell is P×P pixels; origin is the provided base position.
    void addBlock(sf::Vector2f base, int col, int row, int w, int h,
                  sf::Color color);
};
