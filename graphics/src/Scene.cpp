#include "../include/Scene.h"

Scene::Scene() : car(sf::Vector2f(30.0f, 440.0f)) {
    createGrass();
    createSlopeLeft();
    createDarkGrayBridgeLeft();
    createRiver();
    createDarkGrayBridgeRight();
    createSlopeRight();
    createFixedNodes();
}

void Scene::createGrass() {
    grass.setSize(sf::Vector2f(1200, 400));
    grass.setPosition(sf::Vector2f(0, 490));
    grass.setFillColor(sf::Color(110, 130, 70));
}

void Scene::createSlopeLeft() {
    slopeLeft.setPointCount(7);
    slopeLeft.setPoint(0, sf::Vector2f(  0, 440));
    slopeLeft.setPoint(1, sf::Vector2f(150, 440));
    slopeLeft.setPoint(2, sf::Vector2f(270, 600));
    slopeLeft.setPoint(3, sf::Vector2f(248, 604));
    slopeLeft.setPoint(4, sf::Vector2f(224, 608));
    slopeLeft.setPoint(5, sf::Vector2f(200, 612));
    slopeLeft.setPoint(6, sf::Vector2f(  0, 615));
    slopeLeft.setFillColor(sf::Color(122, 122, 122));
}

void Scene::createDarkGrayBridgeLeft() {
    darkGrayBridgeLeft.setPointCount(5);
    darkGrayBridgeLeft.setPoint(0, sf::Vector2f(150, 440));
    darkGrayBridgeLeft.setPoint(1, sf::Vector2f(343, 440));
    // darkGrayBridgeLeft.setPoint(2, sf::Vector2f(343, 460));
    // darkGrayBridgeLeft.setPoint(3, sf::Vector2f(343, 460));
    darkGrayBridgeLeft.setPoint(2, sf::Vector2f(343, 590));
    darkGrayBridgeLeft.setPoint(3, sf::Vector2f(330, 600));
    darkGrayBridgeLeft.setPoint(4, sf::Vector2f(270, 600));
    darkGrayBridgeLeft.setFillColor(sf::Color(50, 50, 50));
}

void Scene::createRiver() {
    river.setPointCount(13);
    river.setPoint(0, sf::Vector2f( 260, 490));
    river.setPoint(1, sf::Vector2f( 376, 495));
    river.setPoint(2, sf::Vector2f( 604, 513));
    river.setPoint(3, sf::Vector2f( 739, 526));
    river.setPoint(4, sf::Vector2f( 802, 546));
    river.setPoint(5, sf::Vector2f(1160, 840));
    river.setPoint(6, sf::Vector2f(1160, 840));
    river.setPoint(7, sf::Vector2f( 417, 840));
    river.setPoint(8, sf::Vector2f( 424, 637));
    river.setPoint(9, sf::Vector2f( 421, 582));
    river.setPoint(10, sf::Vector2f(397, 515));
    river.setPoint(11, sf::Vector2f(303, 501));
    river.setPoint(12, sf::Vector2f(260, 495));
    river.setFillColor(sf::Color(100, 149, 237));
}

void Scene::createDarkGrayBridgeRight() {
    darkGrayBridgeRight.setPointCount(5);
    darkGrayBridgeRight.setPoint(0, sf::Vector2f(1130, 440));
    darkGrayBridgeRight.setPoint(1, sf::Vector2f( 937, 440));
    darkGrayBridgeRight.setPoint(2, sf::Vector2f( 937, 590));
    darkGrayBridgeRight.setPoint(3, sf::Vector2f( 950, 600));
    darkGrayBridgeRight.setPoint(4, sf::Vector2f(1010, 600));
    darkGrayBridgeRight.setFillColor(sf::Color(50, 50, 50));
}

void Scene::createSlopeRight() {
    slopeRight.setPointCount(7);
    slopeRight.setPoint(0, sf::Vector2f(1280, 440));
    slopeRight.setPoint(1, sf::Vector2f(1130, 440));
    slopeRight.setPoint(2, sf::Vector2f(1010, 600));
    slopeRight.setPoint(3, sf::Vector2f(1032, 604));
    slopeRight.setPoint(4, sf::Vector2f(1056, 608));
    slopeRight.setPoint(5, sf::Vector2f(1080, 612));
    slopeRight.setPoint(6, sf::Vector2f(1280, 615));
    slopeRight.setFillColor(sf::Color(122, 122, 122));
}

void Scene::createFixedNodes() {
    // Fixed anchor node at the left end of the bridge deck (x=303, y=440)
    float radius = 4.0f;

    fixedNodeLeft.setRadius(radius);
    fixedNodeLeft.setOrigin(sf::Vector2f(radius, radius));
    fixedNodeLeft.setPosition(sf::Vector2f(343.0f, 440.0f));
    fixedNodeLeft.setFillColor(sf::Color(210, 30, 30));

    // Fixed anchor node at the right end of the bridge deck (x=977, y=440)
    fixedNodeRight.setRadius(radius);
    fixedNodeRight.setOrigin(sf::Vector2f(radius, radius));
    fixedNodeRight.setPosition(sf::Vector2f(937.0f, 440.0f));
    fixedNodeRight.setFillColor(sf::Color(210, 30, 30));
}

void Scene::draw(sf::RenderWindow& window) {
    window.draw(grass);
    window.draw(slopeLeft);
    window.draw(river);
    window.draw(darkGrayBridgeLeft);
    window.draw(slopeRight);
    window.draw(darkGrayBridgeRight);

    // Pixel-art car on the left ground
    car.draw(window);

    // Fixed anchor nodes at bridge endpoints
    window.draw(fixedNodeLeft);
    window.draw(fixedNodeRight);
}
