//WoodBeam.cpp
#include "WoodBeam.h"
#include "Physics.h"
#include "Node.h"
#include <cmath>
#include <algorithm>

WoodBeam::WoodBeam(Node* nodeA,
                 Node* nodeB,
                 float restLength,
                 float maxTensileStress,
                 float maxCompressiveStress,
                 bool isRoad,
                 float cost):
    Element(nodeA, nodeB, restLength),
    maxTensileStress(maxTensileStress),
    maxCompressiveStress(maxCompressiveStress),
    stiffness(ELASTIC_MODULUS * SECTION_AREA / restLength * 0.0004f),  
    colorFactor(0.5f),
    isRoad(isRoad),
    cost(cost)
{}

void WoodBeam::applyForces(){
    if(isBroken) return;

    Vec2 delta = nodeB->position - nodeA->position;
    float length = delta.magnitude();

    if(length == 0.0f) return; //Avoid division by zero

    Vec2 direction = delta / length;
    float deformation = length - restLength; //ΔL = L - L₀
    float forceMag = stiffness * deformation; //F = k · ΔL

    //Newton's third law: equal and opposite forces on nodeA and nodeB
    nodeA->applyForce(direction * forceMag);
    nodeB->applyForce(-direction * forceMag);
    
    updateColorFactor();
    checkBreaking();
}

void WoodBeam::checkBreaking(){
    if(isBroken) return;

    float strain = axialStrain();

    if(strain > maxTensileStress){
        isBroken = true;
        return;
    }

    if(strain < -maxCompressiveStress){
        isBroken = true;
        return;
    }
}

void WoodBeam::updateColorFactor(){
    float strain = axialStrain();

    if(strain >= 0.0f){
        // Tension: colorFactor -> [0.5, 1.0]
        colorFactor = 0.5f + 0.5f * (strain / maxTensileStress);
    }else{
        // Compression: colorFactor -> [0.0, 0.5]
        colorFactor = 0.5f + 0.5f * (strain / maxCompressiveStress);
    }

    // Clamp colorFactor to [0, 1] for visual representation
    colorFactor = std::max(0.0f, std::min(1.0f, colorFactor));
}