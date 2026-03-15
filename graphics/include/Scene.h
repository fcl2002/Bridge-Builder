#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Car.h"

class Scene {
private:
    struct WoodSegment {
        sf::Vector2f start;
        sf::Vector2f end;
    };

    sf::RectangleShape grass;
    sf::ConvexShape slopeLeft;
    sf::ConvexShape darkGrayBridgeLeft;
    sf::RectangleShape bridgeRect;
    sf::ConvexShape river;
    sf::ConvexShape darkGrayBridgeRight;
    sf::ConvexShape slopeRight;

    // Pixel-art car
    Car car;

    // Fixed anchor nodes at each end of the bridge
    sf::CircleShape fixedNodeLeft;
    sf::CircleShape fixedNodeRight;

    std::vector<WoodSegment> woodSegments;
    bool woodDragActive = false;
    float maxWoodLength = 80.0f;
    WoodSegment previewWoodSegment;

    void createGrass();
    void createSlopeLeft();
    void createDarkGrayBridgeLeft();
    void createBridge();
    void createRiver();
    void createDarkGrayBridgeRight();
    void createSlopeRight();
    void createFixedNodes();

public:
    Scene();
    void startWoodSegment(const sf::Vector2f& start, float maxLengthPixels);
    void updateWoodSegmentPreview(const sf::Vector2f& end);
    // Returns debited cost (0 if not created)
    int commitWoodSegment(int& budget, float woodCostPerPixel = 0.5f);
    void cancelWoodSegmentPreview();
    bool isBuildingWood() const { return woodDragActive; }
    void clearWoodSegments();
    void draw(sf::RenderWindow& window);
};
