#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Car.h"

class Scene {
private:
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
    void draw(sf::RenderWindow& window);
};
