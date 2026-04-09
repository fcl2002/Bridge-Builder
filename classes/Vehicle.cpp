//Vehicle.cpp
#include "Vehicle.h"
#include "Node.h"
#include <algorithm>

Vehicle::Vehicle(float weight, float speed):
    weight(weight),
    speed(speed),
    currPos(0.0f),
    hasFallen(false),
    currElem(nullptr)
{}

void Vehicle::update(const std::vector<WoodBeam*>& roadElements){
    if(hasFallen || roadElements.empty()) return;

    //Move the vehicle forward
    currPos += speed * Physics::DT;

    //Find the current road element based on currPos
    float accumulated = 0.0f;
    currElem = nullptr;
    _localOffset = 0.0f;

    for(WoodBeam* beam: roadElements){
        
        if(beam->isBroken){
            hasFallen = true;
            return;
        }
        float len = beam->restLength;
        if(currPos <= accumulated + len){
            currElem     = beam;
            _localOffset = accumulated;
            break;
        }
        accumulated += len;
    }
}

void Vehicle::applyWeightToNodes(){
    if(hasFallen || !currElem || currElem->isBroken){
        hasFallen = true;
        return;
    }

    float t = (currPos - _localOffset) / currElem->restLength;
    t = std::max(0.0f, std::min(1.0f, t)); // Clamp t to [0, 1]

    //Load distribution
    Vec2 forceDown(0.0f, weight);
    currElem->nodeA->applyForce(forceDown * (1.0f - t));
    currElem->nodeB->applyForce(forceDown * t);
}

// added cm
Vec2 Vehicle::getPosition() const {
    if(!currElem) return Vec2(0.0f, 0.0f);

    float t = (currPos - _localOffset) / currElem->restLength;
    t = std::max(0.0f, std::min(1.0f, t));

    return Vec2(
        currElem->nodeA->position.x * (1.0f - t) + currElem->nodeB->position.x * t,
        currElem->nodeA->position.y * (1.0f - t) + currElem->nodeB->position.y * t
    );
}

