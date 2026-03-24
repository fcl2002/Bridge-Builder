#include "../include/Scene.h"
#include "../../classes/WoodBeam.h"

#include <algorithm>
#include <cmath>

namespace {
sf::Vector2f clampSegmentEnd(const sf::Vector2f& start, const sf::Vector2f& end, float maxLength) {
    const sf::Vector2f delta = end - start;
    const float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    if (distance <= maxLength || distance <= 0.0001f) {
        return end;
    }

    const float scale = maxLength / distance;
    return start + delta * scale;
}

void drawWoodSegment(sf::RenderWindow& window, const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color, float thickness = 5.0f) {
    sf::Vector2f delta = end - start;
    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    if (length < 1.0f) return;
    float angle = std::atan2(delta.y, delta.x) * 180.0f / 3.14159265f;
    sf::RectangleShape rect(sf::Vector2f(length, thickness));
    rect.setOrigin(sf::Vector2f(0.f, thickness / 2.0f));
    rect.setPosition(start);
    rect.setRotation(sf::degrees(angle));
    rect.setFillColor(color);
    window.draw(rect);
}
}

Scene::Scene() : car(sf::Vector2f(30.0f, 440.0f)) {
    createGrass();
    createSlopeLeft();
    createDarkGrayBridgeLeft();
    createRiver();
    createDarkGrayBridgeRight();
    createSlopeRight();
    createFixedNodes();
}

Node* Scene::findNode(const sf::Vector2f& pos, float threshold) {
    for (const auto& node : bridge.nodes) { 
        float dx = node->position.x - pos.x;
        float dy = node->position.y - pos.y;
        if (std::sqrt(dx*dx + dy*dy) <= threshold) {
            return node.get(); 
        }
    }
    return nullptr; 
}

std::vector<WoodBeam*> Scene::getRoadBeams() const {
    std::vector<WoodBeam*> roads;
    for (const auto& elem : bridge.elements) {
        if (auto* beam = dynamic_cast<WoodBeam*>(elem.get())) {
            if (beam->isRoad) {
                roads.push_back(beam);
            }
        }
    }

    std::sort(roads.begin(), roads.end(), [](const WoodBeam* a, const WoodBeam* b) {
        float ax = std::min(a->nodeA->position.x, a->nodeB->position.x);
        float bx = std::min(b->nodeA->position.x, b->nodeB->position.x);
        return ax < bx;
    });
    return roads;
}

void Scene::setCarPosition(const sf::Vector2f& target) {
    sf::Vector2f delta = target - carBasePos;
    car.move(delta);
    carBasePos = target;
}

void Scene::resetSimulation() {
    vehicle.currPos = 0.0f;
    vehicle.hasFallen = false;

    // Start the car at the beginning of the road, if available
    auto roads = getRoadBeams();
    if (!roads.empty()) {
        WoodBeam* first = roads.front();
        Vec2 startPos = first->nodeA->position;
        if (first->nodeB->position.x < startPos.x) {
            startPos = first->nodeB->position;
        }
        setCarPosition(sf::Vector2f(startPos.x, startPos.y));
    } else {
        // Default position
        setCarPosition(sf::Vector2f(30.0f, 440.0f));
    }
}

void Scene::simulateStep() {
    // Update vehicle position along the road segments
    auto roads = getRoadBeams();
    vehicle.update(roads);

    // Apply simulation forces (bridge + vehicle)
    std::vector<Vehicle*> vehicles = { &vehicle };
    bridge.step(vehicles);

    // Update car visual position if still on the bridge, otherwise stop
    if (!vehicle.hasFallen) {
        Vec2 pos = vehicle.getPosition();
        setCarPosition(sf::Vector2f(pos.x, pos.y));
    }
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

    // Add corresponding fixed nodes to the physics bridge model
    bridge.addNode(Vec2(343.0f, 440.0f), 1.0f, true);
    bridge.addNode(Vec2(937.0f, 440.0f), 1.0f, true);
}

bool Scene::startWoodSegment(const sf::Vector2f& mousePos, float maxLengthPixels) {
    Node* startNode = findNode(mousePos);
    
    if (!startNode) {
        return false; 
    }

    currentStartNode = startNode;
    maxWoodLength = maxLengthPixels;
    woodDragActive = true;
    
    previewWoodSegment.start = sf::Vector2f(startNode->position.x, startNode->position.y);
    previewWoodSegment.end = previewWoodSegment.start;
    
    return true;
}

void Scene::updateWoodSegmentPreview(const sf::Vector2f& end) {
    if (!woodDragActive) {
        return;
    }

    previewWoodSegment.end = clampSegmentEnd(previewWoodSegment.start, end, maxWoodLength);
}

int Scene::commitWoodSegment(int& budget, float woodCostPerPixel) {
    if (!woodDragActive || !currentStartNode) return 0;

    const sf::Vector2f delta = previewWoodSegment.end - previewWoodSegment.start;
    const float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    int cost = static_cast<int>(std::round(length * woodCostPerPixel));
    
    woodDragActive = false; 
    
    if (length > 1.0f && budget >= cost) {

        Node* endNode = findNode(previewWoodSegment.end);

        if (!endNode) {
            endNode = bridge.addNode(Vec2(previewWoodSegment.end.x, previewWoodSegment.end.y), 1.0f, false);
        }

        if (currentStartNode != endNode) {
            // Detect if beam is part of the road (its horizontal and at bridge height) 
            // Tolerance: ±15° of horizontal AND height between 430-450 pixels (main bridge)
            const sf::Vector2f delta(endNode->position.x - currentStartNode->position.x, 
                                      endNode->position.y - currentStartNode->position.y);
            const float angle = std::atan2(delta.y, delta.x);
            const float angleTol = 15.0f * 3.14159265f / 180.0f; // 15° tolerance
            bool isHorizontal = (std::abs(angle) < angleTol) || (std::abs(angle) > (3.14159265f - angleTol));
            
            // Verify if both nodes are at the bridge height (y ~440)
            float avgY = (endNode->position.y + currentStartNode->position.y) / 2.0f;
            bool isAtBridgeHeight = (avgY > 425.0f && avgY < 465.0f);
            
            bool isRoad = isHorizontal && isAtBridgeHeight;

            bridge.addWoodBeam(currentStartNode, endNode, 0.05f, 0.10f, isRoad);
        budget -= cost;
        woodDragActive = false;
        return cost;
        }
    }

    return 0;
}

void Scene::cancelWoodSegmentPreview() {
    woodDragActive = false;
}

void Scene::clearWoodSegments() {
    // Clear all elements and nodes from the bridge
    bridge.elements.clear();
    bridge.nodes.clear();
    bridge.hasCollapsed = false;
    bridge.totalCost = 0.0f;

    woodSegments.clear();
    woodDragActive = false;
    currentStartNode = nullptr;

    // Reset vehicle and car to initial state
    vehicle.currPos = 0.0f;
    vehicle.hasFallen = false;
    setCarPosition(sf::Vector2f(30.0f, 440.0f));

    // Recreate the fixed anchor nodes
    createFixedNodes();
}

void Scene::draw(sf::RenderWindow& window, bool simRunning) {
    window.draw(grass);
    window.draw(slopeLeft);
    window.draw(river);
    window.draw(darkGrayBridgeLeft);
    window.draw(slopeRight);
    window.draw(darkGrayBridgeRight);

    // Pixel-art car on the left ground
    car.draw(window);

    constexpr float woodThickness = 8.0f; // Altere aqui para mudar a espessura
    for (const WoodSegment& segment : woodSegments) {
        drawWoodSegment(window, segment.start, segment.end, sf::Color(156, 100, 48), woodThickness);
    }
    if (woodDragActive) {
        drawWoodSegment(window, previewWoodSegment.start, previewWoodSegment.end, sf::Color(200, 140, 90), woodThickness);
    }

    // Fixed anchor nodes at bridge endpoints
    window.draw(fixedNodeLeft);
    window.draw(fixedNodeRight);
}
