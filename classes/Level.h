//Level.h
#pragma once
#include "Bridge.h"
#include "Vehicle.h"
#include "Vec2.h"
#include <vector>
#include <memory>
#include <string>

/**
 * @brief Defines level conditions
 * Responsabilities:
 * - Define fixed nodes to anchor the bridge
 * - Limit construction resources (total cost)
 * - Define victory conditions
 */
class Level{
    public:
        std::string name;
        float vehicleStart;
        float vehicleEnd;
        float maxBeamLength;
        float budget;

        Bridge bridge;

    private:
        std::vector<Vec2> _anchorPositions;
        std::vector<Node*> _anchorNodes;

    public:
        /**
         * @param name Level name for display
         * @param anchorPositions List of positions where fixed nodes will be created to anchor the bridge
         * @param vehicleStart Starting position of the vehicle on the road
         * @param vehicleEnd Position the vehicle must reach to win
         * @param maxBeamLength Maximum allowed length for any beam in the bridge
         * @param budget Total cost limit for the bridge construction
         */
        Level(const std::string& name,
              const std::vector<Vec2>& anchorPositions,
              float vehicleStart, float vehicleEnd,
              float maxBeamLength, float budget);

        /**
         * @brief Returns the fixed anchor nodes of the level
         */
        const std::vector<Node*>& getAnchor() const;

        /**
         * @brief Validates if the beam can be added
         * Criteria:
         * - Length must be <= maxBeamLength
         * - Total cost after adding must be <= budget
         * 
         * @param nodeA First node of the beam
         * @param nodeB Second node of the beam
         * @param cost Cost of the beam to be added
         */
        bool canAddBeam(const Node* nodeA, const Node* nodeB, float cost) const;

        /**
         * @brief Tries to add a beam respecting the level constraints
         * @return Pointer to the added beam, or nullptr if it cannot be added
         */
        WoodBeam* tryAddBeam(Node* nodeA, Node* nodeB, float cost = 1.0f, bool isRoad = false);

        /**
         * @brief Creates a vehicle with the level's conditions
         */
        Vehicle createVehicle(float weight, float speed) const;

        /**
         * @brief Returns the remaining budget
         */
        float remainingBudget() const;

        /**
         * @brief Checks victory conditions
         */
        bool isVictory(const Vehicle& vehicle, const std::vector<WoodBeam*>& roadElements) const;
        
        /**
         * @brief Checks if the level is failed
         */
        bool isDefeat(const Vehicle& vehicle) const;
};