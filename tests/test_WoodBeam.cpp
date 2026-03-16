#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../classes/WoodBeam.h"
#include "../classes/Node.h"

TEST_CASE("WoodBeam - constructor initializes derived and base fields") {
    Node nodeA(Vec2(0.0f, 0.0f));
    Node nodeB(Vec2(1.0f, 0.0f));

    WoodBeam beam(&nodeA, &nodeB, 2.0f, 0.2f, 0.3f, true, 12.5f);

    CHECK(beam.nodeA == &nodeA);
    CHECK(beam.nodeB == &nodeB);
    CHECK(beam.restLength == doctest::Approx(2.0f));
    CHECK(beam.isBroken == false);
    CHECK(beam.maxTensileStress == doctest::Approx(0.2f));
    CHECK(beam.maxCompressiveStress == doctest::Approx(0.3f));
    CHECK(beam.stiffness == doctest::Approx(WoodBeam::ELASTIC_MODULUS * WoodBeam::SECTION_AREA / 2.0f));
    CHECK(beam.colorFactor == doctest::Approx(0.5f));
    CHECK(beam.isRoad == doctest::Approx(1.0f));
    CHECK(beam.cost == doctest::Approx(12.5f));
}

TEST_CASE("WoodBeam - applyForces applies equal and opposite axial forces") {
    Node nodeA(Vec2(0.0f, 0.0f));
    Node nodeB(Vec2(2.0f, 0.0f));

    WoodBeam beam(&nodeA, &nodeB, 1.0f, 10.0f, 10.0f, false, 1.0f);
    const float expectedForceMag = beam.stiffness * (2.0f - 1.0f);

    beam.applyForces();

    CHECK(nodeA.forceAccumulator.x == doctest::Approx(expectedForceMag));
    CHECK(nodeA.forceAccumulator.y == doctest::Approx(0.0f));
    CHECK(nodeB.forceAccumulator.x == doctest::Approx(-expectedForceMag));
    CHECK(nodeB.forceAccumulator.y == doctest::Approx(0.0f));
    CHECK(beam.isBroken == false);
}

TEST_CASE("WoodBeam - applyForces does nothing when length is zero") {
    Node nodeA(Vec2(0.0f, 0.0f));
    Node nodeB(Vec2(0.0f, 0.0f));

    WoodBeam beam(&nodeA, &nodeB, 1.0f, 1.0f, 1.0f, false, 1.0f);
    beam.applyForces();

    CHECK(nodeA.forceAccumulator.x == doctest::Approx(0.0f));
    CHECK(nodeA.forceAccumulator.y == doctest::Approx(0.0f));
    CHECK(nodeB.forceAccumulator.x == doctest::Approx(0.0f));
    CHECK(nodeB.forceAccumulator.y == doctest::Approx(0.0f));
}

TEST_CASE("WoodBeam - applyForces is ignored when beam is already broken") {
    Node nodeA(Vec2(0.0f, 0.0f));
    Node nodeB(Vec2(2.0f, 0.0f));

    WoodBeam beam(&nodeA, &nodeB, 1.0f, 10.0f, 10.0f, false, 1.0f);
    beam.isBroken = true;

    beam.applyForces();

    CHECK(nodeA.forceAccumulator.x == doctest::Approx(0.0f));
    CHECK(nodeA.forceAccumulator.y == doctest::Approx(0.0f));
    CHECK(nodeB.forceAccumulator.x == doctest::Approx(0.0f));
    CHECK(nodeB.forceAccumulator.y == doctest::Approx(0.0f));
}

TEST_CASE("WoodBeam - checkBreaking sets broken state on tensile or compressive overload") {
    SUBCASE("tension overload") {
        Node nodeA(Vec2(0.0f, 0.0f));
        Node nodeB(Vec2(1.2f, 0.0f));
        WoodBeam beam(&nodeA, &nodeB, 1.0f, 0.1f, 0.5f, false, 1.0f);

        beam.checkBreaking();

        CHECK(beam.isBroken == true);
    }

    SUBCASE("compression overload") {
        Node nodeA(Vec2(0.0f, 0.0f));
        Node nodeB(Vec2(0.7f, 0.0f));
        WoodBeam beam(&nodeA, &nodeB, 1.0f, 0.5f, 0.2f, false, 1.0f);

        beam.checkBreaking();

        CHECK(beam.isBroken == true);
    }

    SUBCASE("within limits") {
        Node nodeA(Vec2(0.0f, 0.0f));
        Node nodeB(Vec2(1.05f, 0.0f));
        WoodBeam beam(&nodeA, &nodeB, 1.0f, 0.1f, 0.2f, false, 1.0f);

        beam.checkBreaking();

        CHECK(beam.isBroken == false);
    }
}

TEST_CASE("WoodBeam - updateColorFactor maps strain to [0, 1] and clamps") {
    SUBCASE("tension in range") {
        Node nodeA(Vec2(0.0f, 0.0f));
        Node nodeB(Vec2(11.0f, 0.0f));
        WoodBeam beam(&nodeA, &nodeB, 10.0f, 0.2f, 0.2f, false, 1.0f);

        beam.updateColorFactor();

        CHECK(beam.colorFactor == doctest::Approx(0.75f));
    }

    SUBCASE("compression in range") {
        Node nodeA(Vec2(0.0f, 0.0f));
        Node nodeB(Vec2(9.0f, 0.0f));
        WoodBeam beam(&nodeA, &nodeB, 10.0f, 0.2f, 0.2f, false, 1.0f);

        beam.updateColorFactor();

        CHECK(beam.colorFactor == doctest::Approx(0.25f));
    }

    SUBCASE("high tension clamps to 1") {
        Node nodeA(Vec2(0.0f, 0.0f));
        Node nodeB(Vec2(20.0f, 0.0f));
        WoodBeam beam(&nodeA, &nodeB, 10.0f, 0.2f, 0.2f, false, 1.0f);

        beam.updateColorFactor();

        CHECK(beam.colorFactor == doctest::Approx(1.0f));
    }

    SUBCASE("high compression clamps to 0") {
        Node nodeA(Vec2(0.0f, 0.0f));
        Node nodeB(Vec2(0.0f, 0.0f));
        WoodBeam beam(&nodeA, &nodeB, 10.0f, 0.2f, 0.2f, false, 1.0f);

        beam.updateColorFactor();

        CHECK(beam.colorFactor == doctest::Approx(0.0f));
    }
}
