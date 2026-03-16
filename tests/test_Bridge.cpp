#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../classes/Bridge.h"
#include "../classes/Vehicle.h"

TEST_CASE("Bridge - constructor initializes empty state") {
    Bridge bridge;

    CHECK(bridge.nodes.empty());
    CHECK(bridge.elements.empty());
    CHECK(bridge.hasCollapsed == false);
    CHECK(bridge.totalCost == doctest::Approx(0.0f));
}

TEST_CASE("Bridge - addNode stores node and returns pointer") {
    Bridge bridge;

    Node* node = bridge.addNode(Vec2(2.0f, 3.0f), 4.0f, true);

    CHECK(bridge.nodes.size() == 1);
    CHECK(node != nullptr);
    CHECK(node->position.x == doctest::Approx(2.0f));
    CHECK(node->position.y == doctest::Approx(3.0f));
    CHECK(node->mass == doctest::Approx(4.0f));
    CHECK(node->isFix == true);
}

TEST_CASE("Bridge - addWoodBeam computes rest length and total cost") {
    Bridge bridge;
    Node* nodeA = bridge.addNode(Vec2(0.0f, 0.0f));
    Node* nodeB = bridge.addNode(Vec2(3.0f, 4.0f));

    WoodBeam* beam = bridge.addWoodBeam(nodeA, nodeB, 0.5f, 0.6f, true, 12.0f);

    CHECK(bridge.elements.size() == 1);
    CHECK(beam != nullptr);
    CHECK(beam->restLength == doctest::Approx(5.0f));
    CHECK(beam->isRoad == doctest::Approx(1.0f));
    CHECK(bridge.totalCost == doctest::Approx(12.0f));
}

TEST_CASE("Bridge - step applies gravity and updates movable nodes") {
    Bridge bridge;
    Node* node = bridge.addNode(Vec2(0.0f, 10.0f), 2.0f, false);
    const float initialY = node->position.y;

    std::vector<Vehicle*> vehicles;
    bridge.step(vehicles);

    CHECK(node->position.y < initialY);
    CHECK(node->velocity.y < 0.0f);
}

TEST_CASE("Bridge - step does nothing after collapse") {
    Bridge bridge;
    Node* node = bridge.addNode(Vec2(0.0f, 5.0f), 1.0f, false);

    bridge.hasCollapsed = true;
    const float posY = node->position.y;
    const float velY = node->velocity.y;

    std::vector<Vehicle*> vehicles;
    bridge.step(vehicles);

    CHECK(node->position.y == doctest::Approx(posY));
    CHECK(node->velocity.y == doctest::Approx(velY));
}

TEST_CASE("Bridge - collapse is detected when all road beams are broken") {
    Bridge bridge;
    Node* nodeA = bridge.addNode(Vec2(0.0f, 0.0f), 1.0f, false);
    Node* nodeB = bridge.addNode(Vec2(1.0f, 0.0f), 1.0f, false);

    WoodBeam* road = bridge.addWoodBeam(nodeA, nodeB, 1.0f, 1.0f, true, 1.0f);
    road->isBroken = true;

    std::vector<Vehicle*> vehicles;
    bridge.step(vehicles);

    CHECK(bridge.hasCollapsed == true);
}

TEST_CASE("Bridge - no collapse when at least one road beam remains intact") {
    Bridge bridge;
    Node* n0 = bridge.addNode(Vec2(0.0f, 0.0f), 1.0f, false);
    Node* n1 = bridge.addNode(Vec2(1.0f, 0.0f), 1.0f, false);
    Node* n2 = bridge.addNode(Vec2(2.0f, 0.0f), 1.0f, false);

    WoodBeam* road1 = bridge.addWoodBeam(n0, n1, 1.0f, 1.0f, true, 1.0f);
    WoodBeam* road2 = bridge.addWoodBeam(n1, n2, 1.0f, 1.0f, true, 1.0f);
    road1->isBroken = true;
    road2->isBroken = false;

    std::vector<Vehicle*> vehicles;
    bridge.step(vehicles);

    CHECK(bridge.hasCollapsed == false);
}

TEST_CASE("Bridge - vehicle load affects node dynamics during step") {
    Bridge bridgeNoVehicle;
    Node* a0 = bridgeNoVehicle.addNode(Vec2(0.0f, 0.0f), 1.0f, false);
    Node* b0 = bridgeNoVehicle.addNode(Vec2(10.0f, 0.0f), 1.0f, false);
    bridgeNoVehicle.addWoodBeam(a0, b0, 10.0f, 10.0f, true, 1.0f);

    Bridge bridgeWithVehicle;
    Node* a1 = bridgeWithVehicle.addNode(Vec2(0.0f, 0.0f), 1.0f, false);
    Node* b1 = bridgeWithVehicle.addNode(Vec2(10.0f, 0.0f), 1.0f, false);
    WoodBeam* road = bridgeWithVehicle.addWoodBeam(a1, b1, 10.0f, 10.0f, true, 1.0f);

    Vehicle vehicle(100.0f, 0.0f);
    vehicle.currPos = 5.0f;
    std::vector<WoodBeam*> roads = {road};
    vehicle.update(roads);

    std::vector<Vehicle*> withVehicle = {&vehicle};
    std::vector<Vehicle*> noVehicle;

    bridgeNoVehicle.step(noVehicle);
    bridgeWithVehicle.step(withVehicle);

    CHECK(a1->velocity.y < a0->velocity.y);
    CHECK(b1->velocity.y < b0->velocity.y);
}
