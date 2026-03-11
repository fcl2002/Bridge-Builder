#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Grid {
private:
    std::vector<sf::Vector2f> boundaryPoints;
    int cellSize;
    sf::Color gridColor;
    int gridLeft, gridTop, gridWidth, gridHeight;

    void calculateBounds();

public:
    Grid(const std::vector<sf::Vector2f>& boundary, int cellSize = 40);
    void draw(sf::RenderWindow& window);
};
