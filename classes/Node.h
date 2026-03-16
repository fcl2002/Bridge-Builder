//Node.h
#pragma once
#include "Vec2.h"
#include "Physics.h"

/**
 * @brief Node represents a point mass in the simulation
 * Each node has a position, velocity and a force accumulator
 * The dinamics follows Newton's second law: F = m * a
 * The numerical integration is done using the Symplectic Euler method
 */

 class Node{
    public:
        Vec2 position;
        Vec2 velocity;
        Vec2 forceAccumulator;
        float mass;
        bool isFix;

        /**
         * @param position Initial position of the node
         * @param mass Mass of the node
         * @param isFix Whether the node is fixed in space (immovable)
         */
        Node(Vec2 position, float mass = 1.0f, bool isFix = false);

        /**
         * @brief Accumulates a force to the node
         * Forces are accumulated before each integration step
         * 
         * @param force The force vector to be added to the accumulator
         */
        void applyForce(const Vec2& force);

        /**
         * @brief Avances the node state by one time step
         * 
         * Symplectic Euler integration:
         * 1. a = F / m
         * 2. v += a * dt
         * 3. p += v * dt
         * 4. v *= DAMPING
         * 5. Clear force accumulator to prepare for the next step
         */
        void update();

        /**
         * @brief Resets force accumulator to zero
         * Called after each integration step to clear forces for the next frame
         */
        void clearForces();
 };