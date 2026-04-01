//Level.cpp
#include "Level.h"

Level::Level(const std::string& name,
             const std::vector<Vec2>& anchorPositions,
             float vehicleStart, float vehicleEnd,
             float maxBeamLength, float budget):
    name(name),
    vehicleStart(vehicleStart),
    vehicleEnd(vehicleEnd),
    maxBeamLength(maxBeamLength),
    budget(budget){
        for(const Vec2& pos: anchorPositions){
            Node* node = bridge.addNode(pos, 1.0f, true);
            _anchorNodes.push_back(node);
        }
}

const std::vector<Node*>& Level::getAnchor() const{
    return _anchorNodes;
}

bool Level::canAddBeam(const Node* nodeA, const Node* nodeB, float cost) const{
    float length = (nodeB->position - nodeA->position).magnitude();

    if(length > maxBeamLength) return false;
    if(cost > remainingBudget()) return false;

    return true;
}

WoodBeam* Level::tryAddBeam(Node* nodeA, Node* nodeB, float cost, bool isRoad){
    if(!canAddBeam(nodeA, nodeB, cost)) return nullptr;

    float maxTensile = isRoad ? 0.12f : 0.40f;
    float maxCompressive = isRoad ? 0.20f : 0.45f;

    return bridge.addWoodBeam(nodeA, nodeB, maxTensile, maxCompressive, isRoad, cost);
}

Vehicle Level::createVehicle(float weight, float speed) const{
    Vehicle vehicle(weight, speed);
    vehicle.currPos = vehicleStart;
    return vehicle;
}

float Level::remainingBudget() const{
    return budget - bridge.totalCost;
}

bool Level::isVictory(const Vehicle& vehicle,
                      const std::vector<WoodBeam*>& roadElements) const{
    return !vehicle.hasFallen && vehicle.currPos >= vehicleEnd;
}

bool Level::isDefeat(const Vehicle& vehicle) const{
    return vehicle.hasFallen || bridge.hasCollapsed;
}