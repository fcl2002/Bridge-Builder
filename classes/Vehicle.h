//Vehicle.h
#pragma once
#include "Vec2.h"
#include "Physics.h"
#include "WoodBeam.h"
#include <vector>

class WoodBeam; // Forward declaration
class Bridge; // Forward declaration

/**
 * @brief Dynamic load that moves across the road elements of the bridge
 * The weight is distributed between the two nodes of the road beam it is currently on,
 * via Linear interpolation
 */
class Vehicle{
    public:
        float weight; //N, force due to gravity (mass * GRAVITY)
        float speed; //m/s, horizontal speed of the vehicle
        float currPos; //m, current position along the road
        bool hasFallen; //Whether the vehicle has fallen off the bridge

    private:
        WoodBeam* currElem; // Current raod element
        float _localOffset; //m, offset from the beginning of the current road element

    public:
        /**
         * @param weight Force due to gravity (mass * GRAVITY)
         * @param speed Horizontal speed of the vehicle
         */
        Vehicle(float weight, float speed);

        /**
         * @brief Updates the vehicle's position and changes the current road element if necessary
         * @param roadElements Ordered list of road beams
         */
        void update(const std::vector<WoodBeam*>& roadElements);

        /**
         * @brief Distributes the weight of the vehicle to the two nodes of the current road element
         * F_nodeA = weight * (1 - t)
         * F_nodeB = weight * t
         * where t = normalized currPos in the current road element
         */
        void applyWeightToNodes();

        WoodBeam* getCurrentElement() const{
            return currElem;
        }
        /**
         * @brief Returns the world position of the vehicle along the current road element.
         * If the vehicle is not on a road element yet, returns (0, 0).
         */
        Vec2 getPosition() const;
};