#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Scene {
private:
    sf::RectangleShape grass;
    sf::ConvexShape slopeLeft;
    sf::ConvexShape darkGrayBridgeLeft;
    sf::RectangleShape bridgeRect;
    sf::ConvexShape river;
    sf::ConvexShape darkGrayBridgeRight;
    sf::ConvexShape slopeRight;

    void createGrass();
    void createSlopeLeft();
    void createDarkGrayBridgeLeft();
    void createBridge();
    void createRiver();
    void createDarkGrayBridgeRight();
    void createSlopeRight();

public:
    Scene();
    void draw(sf::RenderWindow& window);
};
