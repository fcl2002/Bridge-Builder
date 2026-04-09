//Node.cpp
#include "Node.h"
#include <iostream>
Node::Node(Vec2 position, float mass, bool isFix):
    position(position),
    initialPosition(position),
    velocity(Vec2()),
    forceAccumulator(Vec2()),
    mass(mass),
    isFix(isFix),
    accumulatedMass(0.0f)
{}

void Node::applyForce(const Vec2& force){
    if(isFix) return; //Fixed nodes do not accumulate forces
    forceAccumulator += force;
}

void Node::update(){
    if(isFix) return; //Fixed nodes do not move

    float effectiveMass = mass + accumulatedMass;
   

    //1. a = F / m (Newton's second law)
    Vec2 acceleration = forceAccumulator / effectiveMass;

    //2. v += a * dt (Symplectic Euler velocity update)
    velocity += acceleration * Physics::DT;

    //3. v *= DAMPING (Apply damping to simulate energy loss before updating position)
    velocity *= Physics::DAMPING;

    //4. p += v * dt (Symplectic Euler position update)
    position += velocity * Physics::DT;

    //5. Clear force accumulator for the next frame
    clearForces();
}

void Node::clearForces(){
    forceAccumulator.zero();
    accumulatedMass = 0.0f;  

}