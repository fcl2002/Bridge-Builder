// WoodBeam.cpp
#include "WoodBeam.h"
#include "Physics.h"
#include "Node.h"
#include <cmath>
#include <algorithm>
#include <iostream>


static int ruptureOrder = 0;

WoodBeam::WoodBeam(Node *nodeA,
                   Node *nodeB,
                   float restLength,
                   float forceBeamMax,
                   bool isRoad,
                   float cost) : Element(nodeA, nodeB, restLength),
                                 stiffness(Physics::K_STIFFNESS),
                                 forceBeamMax(forceBeamMax),
                                 colorFactor(0.5f),
                                 isRoad(isRoad),
                                 cost(cost)
{}

void WoodBeam::applyForces()
{
    if (isBroken)
        return;

    Vec2 delta = nodeB->position - nodeA->position;
    float length = delta.magnitude();
    if (length < 0.0001f)
        return;

    Vec2 direction = delta / length;
    float deformation = length - restLength;                // ΔL = L - L₀
    Vec2 forceBeam = direction * (stiffness * deformation); // F = k · ΔL (vecteur)

  
    float beamMass = Physics::MASS_PER_LENGTH * restLength;
    Vec2 gravity(0.0f, beamMass * Physics::GRAVITY / 2.0f);

    // 3 law of Newton: action = -reaction, plus gravity on each node
    nodeA->applyForce(forceBeam + gravity);
    nodeB->applyForce(-forceBeam + gravity);

    // Distribute beam mass to nodes for inertia calculation (half mass to each node)
    float halfMass = beamMass / 2.0f;
    nodeA->addMass(halfMass);
    nodeB->addMass(halfMass);

    updateColorFactor();
}

void WoodBeam::checkBreaking()
{
    if (isBroken)
        return;

    
    Vec2 delta = nodeB->position - nodeA->position;
    float length = delta.magnitude();
    if (length < 0.0001f)
        return;

    float forceMag = stiffness * std::abs(length - restLength);
    colorFactor = forceMag / forceBeamMax;

    if(colorFactor >= 1.0f){
        colorFactor = 1.0f;
        isBroken    = true;
        ruptureOrder++;
        std::cout << "[RUPTURE #" << ruptureOrder 
                  << " ]isRoad=" << isRoad
                  << " forceMag=" << forceMag
                  << " deformation=" << (length - restLength)
                  << " restLength=" << restLength
                  << " dispA=" << (nodeA->position.y - nodeA->initialPosition.y)
                  << " dispB=" << (nodeB->position.y - nodeB->initialPosition.y)
                  << std::endl;
    }
}

void WoodBeam::updateColorFactor()
{
    Vec2 delta = nodeB->position - nodeA->position;
    float length = delta.magnitude();
    if (length < 0.0001f)
        return;

    float forceMag = stiffness * std::abs(length - restLength);
    colorFactor = std::min(1.0f, forceMag / forceBeamMax);
}