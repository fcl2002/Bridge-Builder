//Bridge.h
#pragma once
#include "Node.h"
#include "WoodBeam.h"
#include <vector>
#include <memory>

class Vehicle; // Forward declaration

/**
 * @brief Manages the physical simulation of the bridge
 * Responsibilities:
 * - Manage nodes and elements (beams) lifecycles
 * - Execute the simulation loop in the correct order
 * - Detect structural collapse * 
 */
class Bridge{
    public: std::vector<std::unique_ptr<Node>> nodes;
    public: std::vector<std::unique_ptr<Element>> elements;
    bool hasCollapsed;
    float totalCost;

    Bridge();

    /**
     * @brief Adds a node to the bridge
     * @param position Initial position of the node
     * @param mass Mass of the node
     * @param isFix Whether the node is fixed in space
     * @return Pointer to the newly created node
     */
    Node* addNode(Vec2 position, float mass = 1.0f, bool isFix = false);

    /**
     * @brief Adds a wood beam element between two nodes
     * Restlength is calculated automatically based on initial node positions
     * @param nodeA Pointer to the first node
     * @param nodeB Pointer to the second node
     * @return Pointer to the newly created wood beam element
     */
    WoodBeam* addWoodBeam(Node* nodeA, Node* nodeB,
                       float maxTensileStress = 0.05f,
                       float maxCompressiveStress = 0.10f,
                       bool isRoad = false,
                       float cost = 1.0f);

    /**
     * @brief Executes one simulation step:
     * 1. clearForces() - Clear all forces from the previous step
     * 2. applyGravity() - Apply gravity
     * 3. applyForces() - Each element applies its forces
     * 4. update() - Each node integrates its position and velocity
     * 5. checkCollapse() - Check global collapse
     */
    void step(const std::vector<Vehicle*>& vehicles);

    private:
        void clearForces();
        void applyGravity();
        void applyElementForces();
        void updateNodes();
        void checkCollapse();
        void applyVehicleForces(const std::vector<Vehicle*>& vehicles);
};