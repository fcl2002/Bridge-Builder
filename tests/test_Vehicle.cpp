#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../classes/Vehicle.h"
#include "../classes/Node.h"
#include "../classes/WoodBeam.h"

TEST_CASE("Vehicle - constructor initializes state") {
    Vehicle vehicle(120.0f, 7.5f);

    CHECK(vehicle.weight == doctest::Approx(120.0f));
    CHECK(vehicle.speed == doctest::Approx(7.5f));
    CHECK(vehicle.currPos == doctest::Approx(0.0f));
    CHECK(vehicle.hasFallen == false);
    CHECK(vehicle.getCurrentElement() == nullptr);
}

TEST_CASE("Vehicle - update with empty road list keeps initial state") {
    Vehicle vehicle(100.0f, 10.0f);
    std::vector<WoodBeam*> roads;

    vehicle.update(roads);

    CHECK(vehicle.currPos == doctest::Approx(0.0f));
    CHECK(vehicle.hasFallen == false);
    CHECK(vehicle.getCurrentElement() == nullptr);
}

TEST_CASE("Vehicle - update advances position and selects current element") {
    Node n0(Vec2(0.0f, 0.0f));
    Node n1(Vec2(10.0f, 0.0f));
    WoodBeam road(&n0, &n1, 10.0f, 1.0f, 1.0f, true, 1.0f);
    std::vector<WoodBeam*> roads = {&road};

    Vehicle vehicle(100.0f, 5.0f);
    vehicle.update(roads);

    CHECK(vehicle.currPos == doctest::Approx(5.0f * Physics::DT));
    CHECK(vehicle.getCurrentElement() == &road);
    CHECK(vehicle.hasFallen == false);
}

TEST_CASE("Vehicle - update marks fallen when encountering broken element") {
    Node n0(Vec2(0.0f, 0.0f));
    Node n1(Vec2(10.0f, 0.0f));
    WoodBeam road(&n0, &n1, 10.0f, 1.0f, 1.0f, true, 1.0f);
    road.isBroken = true;
    std::vector<WoodBeam*> roads = {&road};

    Vehicle vehicle(100.0f, 1.0f);
    vehicle.update(roads);

    CHECK(vehicle.hasFallen == true);
}

TEST_CASE("Vehicle - applyWeightToNodes distributes force by interpolation") {
    SUBCASE("at beam start, all weight goes to nodeA") {
        Node n0(Vec2(0.0f, 0.0f));
        Node n1(Vec2(10.0f, 0.0f));
        WoodBeam road(&n0, &n1, 10.0f, 1.0f, 1.0f, true, 1.0f);
        std::vector<WoodBeam*> roads = {&road};

        Vehicle vehicle(100.0f, 0.0f);
        vehicle.currPos = 0.0f;
        vehicle.update(roads);
        vehicle.applyWeightToNodes();

        CHECK(n0.forceAccumulator.y == doctest::Approx(-100.0f));
        CHECK(n1.forceAccumulator.y == doctest::Approx(0.0f));
    }

    SUBCASE("at beam midpoint, weight is split equally") {
        Node n0(Vec2(0.0f, 0.0f));
        Node n1(Vec2(10.0f, 0.0f));
        WoodBeam road(&n0, &n1, 10.0f, 1.0f, 1.0f, true, 1.0f);
        std::vector<WoodBeam*> roads = {&road};

        Vehicle vehicle(100.0f, 0.0f);
        vehicle.currPos = 5.0f;
        vehicle.update(roads);
        vehicle.applyWeightToNodes();

        CHECK(n0.forceAccumulator.y == doctest::Approx(-50.0f));
        CHECK(n1.forceAccumulator.y == doctest::Approx(-50.0f));
    }
}

TEST_CASE("Vehicle - applyWeightToNodes sets fallen when no valid element is selected") {
    Vehicle vehicle(100.0f, 0.0f);

    vehicle.applyWeightToNodes();

    CHECK(vehicle.hasFallen == true);
}


