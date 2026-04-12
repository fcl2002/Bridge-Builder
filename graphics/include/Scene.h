#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Car.h"
#include "../../classes/Bridge.h"
#include "../../classes/Vehicle.h"

class Scene {
private:

    Bridge bridge;
    Node* currentStartNode = nullptr;


    struct WoodSegment {
        sf::Vector2f start;
        sf::Vector2f end;
    };

    sf::RectangleShape grass;
    sf::ConvexShape slopeLeft;
    sf::ConvexShape darkGrayBridgeLeft;
    sf::RectangleShape bridgeRect;
    sf::ConvexShape river;
    sf::ConvexShape darkGrayBridgeRight;
    sf::ConvexShape slopeRight;
    sf::Texture flagTexture;
    std::unique_ptr<sf::Sprite> flagSprite;
    bool flagLoaded = false;

    // Pixel-art car (rendered on the bridge)
    Car car;
    sf::Vector2f carBasePos;
    Vehicle carVehicle;
    bool hasCarVehicle = true;

    sf::Texture truckTexture;
    std::unique_ptr<sf::Sprite> truckSprite;
    bool truckLoaded = false;
    sf::Vector2f truckBasePos;
    Vehicle truckVehicle;
    bool hasTruckVehicle = false;

    // Fixed anchor nodes at each end of the bridge
    sf::CircleShape fixedNodeLeft;
    sf::CircleShape fixedNodeRight;
    std::vector<sf::Vector2f> extraFixedNodePositions;
    std::vector<Node*> extraFixedNodes;

    std::vector<WoodSegment> woodSegments;
    bool woodDragActive = false;
    float maxWoodLength = 80.0f;
    WoodSegment previewWoodSegment;

    void createTruck();

    void createGrass();
    void createSlopeLeft();
    void createDarkGrayBridgeLeft();
    void createBridge();
    void createRiver();
    void createDarkGrayBridgeRight();
    void createSlopeRight();
    void createFlag();
    void createFixedNodes();
    void rebuildExtraFixedNodes();

    // Helpers for simulating the vehicle and its position along the road
    std::vector<WoodBeam*> getRoadBeams() const;
    void setCarPosition(const sf::Vector2f& target);
    void setTruckPosition(const sf::Vector2f& target);

public:
    Scene(bool enableCarVehicle = true, bool enableTruckVehicle = false);
    Node* addFixedSupportNode(const sf::Vector2f& position);
    bool startWoodSegment(const sf::Vector2f& start, float maxLengthPixels);
    void updateWoodSegmentPreview(const sf::Vector2f& end);
    // Returns debited cost (0 if not created)
    Bridge& getBridge() {
        return bridge;
    }
    Car& getCar() {
        return car;
    }
    Node* findNode(const sf::Vector2f& pos, float threshold = 25.0f);
    
    int commitWoodSegment(int& budget, float woodCostPerPixel = 0.5f);
    void cancelWoodSegmentPreview();
    bool isBuildingWood() const { return woodDragActive; }
    void clearWoodSegments();

    // Simulation helpers
    void resetSimulation();
    void simulateStep();

    void draw(sf::RenderWindow& window, bool simRunning = false);
};
