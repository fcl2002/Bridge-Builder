#include "../include/Scene.h"

Scene::Scene() {
    createGrass();
    createSlopeLeft();
    createDarkGrayBridgeLeft();
    createBridge();
    createRiver();
    createDarkGrayBridgeRight();
    createSlopeRight();
}

void Scene::createGrass() {
    grass.setSize(sf::Vector2f(1200, 400));
    grass.setPosition(sf::Vector2f(0, 490));
    grass.setFillColor(sf::Color(110, 130, 70));
}

void Scene::createSlopeLeft() {
    slopeLeft.setPointCount(7);
    slopeLeft.setPoint(0, sf::Vector2f(  0, 400+40));
    slopeLeft.setPoint(1, sf::Vector2f(150, 400+40));
    slopeLeft.setPoint(2, sf::Vector2f(270, 560+40));
    slopeLeft.setPoint(3, sf::Vector2f(248, 564+40));
    slopeLeft.setPoint(4, sf::Vector2f(224, 568+40));
    slopeLeft.setPoint(5, sf::Vector2f(200, 572+40));
    slopeLeft.setPoint(6, sf::Vector2f(  0, 575+40));
    slopeLeft.setFillColor(sf::Color(143, 163, 90));
}

void Scene::createDarkGrayBridgeLeft() {
    darkGrayBridgeLeft.setPointCount(7);
    darkGrayBridgeLeft.setPoint(0, sf::Vector2f(150, 400+40));
    darkGrayBridgeLeft.setPoint(1, sf::Vector2f(303, 400+40));
    darkGrayBridgeLeft.setPoint(2, sf::Vector2f(303, 420+40));
    darkGrayBridgeLeft.setPoint(3, sf::Vector2f(343, 420+40));
    darkGrayBridgeLeft.setPoint(4, sf::Vector2f(343, 550+40));
    darkGrayBridgeLeft.setPoint(5, sf::Vector2f(330, 560+40));
    darkGrayBridgeLeft.setPoint(6, sf::Vector2f(270, 560+40));
    darkGrayBridgeLeft.setFillColor(sf::Color(50, 50, 50));
}

void Scene::createBridge() {
    bridgeRect.setSize(sf::Vector2f(674, 20));
    bridgeRect.setPosition(sf::Vector2f(303, 440));
    bridgeRect.setFillColor(sf::Color(80, 80, 80));
}

void Scene::createRiver() {
    river.setPointCount(13);
    river.setPoint(0, sf::Vector2f( 260, 450+40));
    river.setPoint(1, sf::Vector2f( 376, 455+40));
    river.setPoint(2, sf::Vector2f( 604, 473+40));
    river.setPoint(3, sf::Vector2f( 739, 486+40));
    river.setPoint(4, sf::Vector2f( 802, 506+40));
    river.setPoint(5, sf::Vector2f(1160, 800+40));
    river.setPoint(6, sf::Vector2f(1160, 800+40));
    river.setPoint(7, sf::Vector2f( 417, 800+40));
    river.setPoint(8, sf::Vector2f( 424, 597+40));
    river.setPoint(9, sf::Vector2f( 421, 542+40));
    river.setPoint(10, sf::Vector2f(397, 475+40));
    river.setPoint(11, sf::Vector2f(303, 461+40));
    river.setPoint(12, sf::Vector2f(260, 455+40));
    river.setFillColor(sf::Color(100, 149, 237));
}

void Scene::createDarkGrayBridgeRight() {
    darkGrayBridgeRight.setPointCount(7);
    darkGrayBridgeRight.setPoint(0, sf::Vector2f(1200-150+80, 400+40));
    darkGrayBridgeRight.setPoint(1, sf::Vector2f(1200-303+80, 400+40));
    darkGrayBridgeRight.setPoint(2, sf::Vector2f(1200-303+80, 420+40));
    darkGrayBridgeRight.setPoint(3, sf::Vector2f(1200-343+80, 420+40));
    darkGrayBridgeRight.setPoint(4, sf::Vector2f(1200-343+80, 550+40));
    darkGrayBridgeRight.setPoint(5, sf::Vector2f(1200-330+80, 560+40));
    darkGrayBridgeRight.setPoint(6, sf::Vector2f(1200-270+80, 560+40));
    darkGrayBridgeRight.setFillColor(sf::Color(50, 50, 50));
}

void Scene::createSlopeRight() {
    slopeRight.setPointCount(7);
    slopeRight.setPoint(0, sf::Vector2f(1200 + 80, 400+40));
    slopeRight.setPoint(1, sf::Vector2f(1200 - 150 + 80, 400+40));
    slopeRight.setPoint(2, sf::Vector2f(1200 - 270 + 80, 560+40));
    slopeRight.setPoint(3, sf::Vector2f(1200 - 248 + 80, 564+40));
    slopeRight.setPoint(4, sf::Vector2f(1200 - 224 + 80, 568+40));
    slopeRight.setPoint(5, sf::Vector2f(1200 - 200 + 80, 572+40));
    slopeRight.setPoint(6, sf::Vector2f(1200 + 80, 575+40));
    slopeRight.setFillColor(sf::Color(143, 163, 90));
}

void Scene::draw(sf::RenderWindow& window) {
    window.draw(grass);
    window.draw(slopeLeft);
    window.draw(river);
    window.draw(darkGrayBridgeLeft);
    window.draw(slopeRight);
    window.draw(darkGrayBridgeRight);
    window.draw(bridgeRect);
}
