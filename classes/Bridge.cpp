//Bridge.cpp
#include "Bridge.h"
#include "Physics.h"
#include "Vehicle.h"

Bridge::Bridge():
    hasCollapsed(false), totalCost(0.0f)
{}

Node* Bridge::addNode(Vec2 position, float mass, bool isFix){
    nodes.push_back(std::make_unique<Node>(position, mass, isFix));
    return nodes.back().get();
}

WoodBeam* Bridge::addWoodBeam(Node* nodeA, Node* nodeB,
                       float maxTensileStress,
                       float maxCompressiveStress,
                       bool isRoad,
                       float cost){
    float restLength = (nodeB->position - nodeA->position).magnitude();

    auto beam = std::make_unique<WoodBeam>(
        nodeA, nodeB, restLength,
        maxTensileStress, maxCompressiveStress,
        isRoad, cost
    );

    totalCost += cost;

    WoodBeam* ptr = beam.get();
    elements.push_back(std::move(beam));
    return ptr;
}

void Bridge::step(const std::vector<Vehicle*>& vehicles, const std::vector<WoodBeam*>& roadElements){
    if(hasCollapsed) return;

    clearForces();
    applyGravity();
    applyElementForces();
    applyVehicleForces(vehicles, roadElements);
    updateNodes();
    checkCollapse();
}

void Bridge::step(const std::vector<Vehicle*>& vehicles){
    std::vector<WoodBeam*> roads;
    for(auto& elem : elements){
        if(auto* beam = dynamic_cast<WoodBeam*>(elem.get())){
            if(beam->isRoad){
                roads.push_back(beam);
            }
        }
    }
    step(vehicles, roads);
}

void Bridge::applyVehicleForces(const std::vector<Vehicle*>& vehicles, const std::vector<WoodBeam*>& roadElements){
    for(auto* vehicle: vehicles){
        vehicle->applyWeightToNodes(roadElements);
    }
}

void Bridge::clearForces(){
    for(auto& node: nodes){
        node->clearForces();
    }
}

void Bridge::applyGravity(){
    for(auto& node: nodes){
        if(node->isFix) continue;
        Vec2 gravity(0.0f, Physics::GRAVITY * node->mass);
        node->applyForce(gravity);
    }
}

void Bridge::applyElementForces(){
    for(auto& element: elements){
        element->applyForces();
    }
}

void Bridge::applyVehicleForces(const std::vector<Vehicle*>& vehicles){
    for(auto* vehicle: vehicles){
        vehicle->applyWeightToNodes();
    }
}

void Bridge::updateNodes(){
    for(auto& node: nodes){
        node->update();
    }
}

void Bridge::checkCollapse(){
    int roadCount = 0;
    int brokenRoads = 0;
    
    for(auto& element: elements){
        WoodBeam* beam = dynamic_cast<WoodBeam*>(element.get());
        if(!beam || !beam->isRoad) continue;
        roadCount++;
        if(beam->isBroken) brokenRoads++;
    }

    if(roadCount > 0 && roadCount == brokenRoads){
        hasCollapsed = true;
    }
}