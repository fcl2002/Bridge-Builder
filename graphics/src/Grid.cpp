#include "../include/Grid.h"
#include <algorithm>
#include <cmath>

Grid::Grid(const std::vector<sf::Vector2f>& boundary, int cellSize)
    : boundaryPoints(boundary), cellSize(cellSize), gridColor(0, 0, 0, 80) {
    calculateBounds();
}

void Grid::calculateBounds() {
    if (boundaryPoints.empty()) return;
    
    float minX = boundaryPoints[0].x, maxX = boundaryPoints[0].x;
    float minY = boundaryPoints[0].y, maxY = boundaryPoints[0].y;
    
    for (const auto& point : boundaryPoints) {
        if (point.x < minX) minX = point.x;
        if (point.x > maxX) maxX = point.x;
        if (point.y < minY) minY = point.y;
        if (point.y > maxY) maxY = point.y;
    }
    
    gridLeft = (int)minX;
    gridTop = (int)minY;
    gridWidth = (int)(maxX - minX);
    gridHeight = (int)(maxY - minY);
}

void Grid::draw(sf::RenderWindow& window) {
    // Draw vertical lines
    for (int x = gridLeft; x <= gridLeft + gridWidth; x += cellSize) {
        std::vector<float> intersectionY;
        
        for (size_t i = 0; i < boundaryPoints.size(); i++) {
            sf::Vector2f p1 = boundaryPoints[i];
            sf::Vector2f p2 = boundaryPoints[(i + 1) % boundaryPoints.size()];
            
            if ((p1.x <= x && p2.x >= x) || (p1.x >= x && p2.x <= x)) {
                if (std::abs(p2.x - p1.x) > 0.001f) {
                    float y = p1.y + (p2.y - p1.y) * (x - p1.x) / (p2.x - p1.x);
                    intersectionY.push_back(y);
                }
            }
        }
        
        std::sort(intersectionY.begin(), intersectionY.end());
        for (size_t i = 0; i + 1 < intersectionY.size(); i += 2) {
            sf::Vertex line[2];
            line[0].position = sf::Vector2f((float)x, intersectionY[i]);
            line[0].color = gridColor;
            line[1].position = sf::Vector2f((float)x, intersectionY[i + 1]);
            line[1].color = gridColor;
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
    }
    
    // Draw horizontal lines
    for (int y = gridTop; y <= gridTop + gridHeight; y += cellSize) {
        std::vector<float> intersectionX;
        
        for (size_t i = 0; i < boundaryPoints.size(); i++) {
            sf::Vector2f p1 = boundaryPoints[i];
            sf::Vector2f p2 = boundaryPoints[(i + 1) % boundaryPoints.size()];
            
            if ((p1.y <= y && p2.y >= y) || (p1.y >= y && p2.y <= y)) {
                if (std::abs(p2.y - p1.y) > 0.001f) {
                    float x = p1.x + (p2.x - p1.x) * (y - p1.y) / (p2.y - p1.y);
                    intersectionX.push_back(x);
                }
            }
        }
        
        std::sort(intersectionX.begin(), intersectionX.end());
        for (size_t i = 0; i + 1 < intersectionX.size(); i += 2) {
            sf::Vertex line[2];
            line[0].position = sf::Vector2f(intersectionX[i], (float)y);
            line[0].color = gridColor;
            line[1].position = sf::Vector2f(intersectionX[i + 1], (float)y);
            line[1].color = gridColor;
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
    }
    
    // Draw boundary outline
    if (boundaryPoints.size() >= 3) {
        for (size_t i = 0; i < boundaryPoints.size(); i++) {
            size_t next = (i + 1) % boundaryPoints.size();
            sf::Vertex boundaryLine[2];
            boundaryLine[0].position = boundaryPoints[i];
            boundaryLine[0].color = sf::Color(0, 200, 0, 255);
            boundaryLine[1].position = boundaryPoints[next];
            boundaryLine[1].color = sf::Color(0, 200, 0, 255);
            window.draw(boundaryLine, 2, sf::PrimitiveType::Lines);
        }
    }
}
