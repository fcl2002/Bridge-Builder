#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../classes/Level.h"

TEST_CASE("Level - constructor stores configuration and creates fixed anchors") {
    std::vector<Vec2> anchors = {Vec2(0.0f, 0.0f), Vec2(4.0f, 0.0f)};

    Level level("Tutorial", anchors, 1.0f, 20.0f, 5.0f, 12.0f);

    CHECK(level.name == "Tutorial");
    CHECK(level.vehicleStart == doctest::Approx(1.0f));
    CHECK(level.vehicleEnd == doctest::Approx(20.0f));
    CHECK(level.maxBeamLength == doctest::Approx(5.0f));
    CHECK(level.budget == doctest::Approx(12.0f));
    CHECK(level.bridge.nodes.size() == 2);
    CHECK(level.getAnchor().size() == 2);
    CHECK(level.getAnchor()[0]->isFix == true);
    CHECK(level.getAnchor()[1]->isFix == true);
    CHECK(level.getAnchor()[0]->position.x == doctest::Approx(0.0f));
    CHECK(level.getAnchor()[1]->position.x == doctest::Approx(4.0f));
}

TEST_CASE("Level - canAddBeam enforces max length and remaining budget") {
    SUBCASE("beam within constraints is allowed") {
        Level level("Budget", {Vec2(0.0f, 0.0f), Vec2(4.0f, 0.0f)}, 0.0f, 10.0f, 5.0f, 10.0f);

        CHECK(level.canAddBeam(level.getAnchor()[0], level.getAnchor()[1], 3.0f) == true);
    }

    SUBCASE("beam longer than allowed is rejected") {
        Level level("Length", {Vec2(0.0f, 0.0f), Vec2(8.0f, 0.0f)}, 0.0f, 10.0f, 5.0f, 10.0f);

        CHECK(level.canAddBeam(level.getAnchor()[0], level.getAnchor()[1], 3.0f) == false);
    }

    SUBCASE("beam that exceeds remaining budget is rejected") {
        Level level("Budget", {Vec2(0.0f, 0.0f), Vec2(4.0f, 0.0f)}, 0.0f, 10.0f, 5.0f, 10.0f);
        level.tryAddBeam(level.getAnchor()[0], level.getAnchor()[1], 8.0f, false);

        CHECK(level.remainingBudget() == doctest::Approx(2.0f));
        CHECK(level.canAddBeam(level.getAnchor()[0], level.getAnchor()[1], 3.0f) == false);
    }
}

TEST_CASE("Level - tryAddBeam adds beams only when constraints are satisfied") {
    SUBCASE("successful addition updates bridge and budget") {
        Level level("Build", {Vec2(0.0f, 0.0f), Vec2(4.0f, 0.0f)}, 0.0f, 10.0f, 5.0f, 10.0f);

        WoodBeam* beam = level.tryAddBeam(level.getAnchor()[0], level.getAnchor()[1], 3.5f, true);

        REQUIRE(beam != nullptr);
        CHECK(level.bridge.elements.size() == 1);
        CHECK(level.bridge.totalCost == doctest::Approx(3.5f));
        CHECK(level.remainingBudget() == doctest::Approx(6.5f));
        CHECK(beam->isRoad == doctest::Approx(1.0f));
    }

    SUBCASE("failed addition leaves bridge unchanged") {
        Level level("Build", {Vec2(0.0f, 0.0f), Vec2(8.0f, 0.0f)}, 0.0f, 10.0f, 5.0f, 10.0f);

        WoodBeam* beam = level.tryAddBeam(level.getAnchor()[0], level.getAnchor()[1], 3.5f, true);

        CHECK(beam == nullptr);
        CHECK(level.bridge.elements.empty());
        CHECK(level.bridge.totalCost == doctest::Approx(0.0f));
        CHECK(level.remainingBudget() == doctest::Approx(10.0f));
    }
}

TEST_CASE("Level - createVehicle returns a vehicle with requested parameters") {
    Level level("Vehicle", {Vec2(0.0f, 0.0f), Vec2(4.0f, 0.0f)}, 2.0f, 14.0f, 5.0f, 10.0f);

    Vehicle vehicle = level.createVehicle(250.0f, 6.0f);

    CHECK(vehicle.weight == doctest::Approx(250.0f));
    CHECK(vehicle.speed == doctest::Approx(6.0f));
    CHECK(vehicle.currPos == doctest::Approx(2.0f)); // starts at vehicleStart
    CHECK(vehicle.hasFallen == false);
}

TEST_CASE("Level - victory and defeat conditions reflect vehicle and bridge state") {
    Level level("Outcome", {Vec2(0.0f, 0.0f), Vec2(4.0f, 0.0f)}, 0.0f, 4.0f, 5.0f, 10.0f);
    WoodBeam* road = level.tryAddBeam(level.getAnchor()[0], level.getAnchor()[1], 2.0f, true);
    REQUIRE(road != nullptr);
    std::vector<WoodBeam*> roads = {road};

    SUBCASE("victory when vehicle reaches vehicleEnd without falling") {
        Vehicle vehicle = level.createVehicle(100.0f, 1.0f);
        vehicle.currPos = level.vehicleEnd;

        CHECK(level.isVictory(vehicle, roads) == true);
        CHECK(level.isDefeat(vehicle) == false);
    }

    SUBCASE("no victory before reaching vehicleEnd") {
        Vehicle vehicle = level.createVehicle(100.0f, 1.0f);
        vehicle.currPos = level.vehicleEnd - 0.1f;

        CHECK(level.isVictory(vehicle, roads) == false);
    }

    SUBCASE("no victory and defeat when vehicle has fallen") {
        Vehicle vehicle = level.createVehicle(100.0f, 1.0f);
        vehicle.currPos = level.vehicleEnd;
        vehicle.hasFallen = true;

        CHECK(level.isVictory(vehicle, roads) == false);
        CHECK(level.isDefeat(vehicle) == true);
    }

    SUBCASE("defeat when bridge has collapsed") {
        Vehicle vehicle = level.createVehicle(100.0f, 1.0f);
        level.bridge.hasCollapsed = true;

        CHECK(level.isDefeat(vehicle) == true);
    }
}