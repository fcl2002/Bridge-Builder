//Node.cpp
#include "Node.h"

Node::Node(Vec2 position, float mass, bool isFix):
    position(position),
    velocity(Vec2()),
    forceAccumulator(Vec2()),
    mass(mass),
    isFix(isFix)
{}

void Node::applyForce(const Vec2& force){
    if(isFix) return; //Fixed nodes do not accumulate forces
    forceAccumulator += force;
}

void Node::update(){
    if(isFix) return; //Fixed nodes do not move

    //1. a = F / m (Newton's second law)
    Vec2 acceleration = forceAccumulator / mass;

    //2. v += a * dt (Symplectic Euler velocity update)
    velocity += acceleration * Physics::DT;

    //3. p += v * dt (Symplectic Euler position update)
    position += velocity * Physics::DT;

    //4. v *= DAMPING (Apply damping to simulate energy loss)
    velocity *= Physics::DAMPING;

    //5. Clear force accumulator for the next frame
    clearForces();
}

void Node::clearForces(){
    forceAccumulator.zero();
}