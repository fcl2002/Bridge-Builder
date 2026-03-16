#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../classes/Node.h"

TEST_CASE("Node - constructor initializes state") {
    Vec2 initialPos(3.0f, 4.0f);
    Node node(initialPos, 2.5f, false);

    CHECK(node.position.x == 3.0f);
    CHECK(node.position.y == 4.0f);
    CHECK(node.velocity.x == 0.0f);
    CHECK(node.velocity.y == 0.0f);
    CHECK(node.forceAccumulator.x == 0.0f);
    CHECK(node.forceAccumulator.y == 0.0f);
    CHECK(node.mass == 2.5f);
    CHECK(node.isFix == false);
}

TEST_CASE("Node - applyForce accumulates on movable node") {
    Node node(Vec2(0.0f, 0.0f), 1.0f, false);

    node.applyForce(Vec2(1.0f, 2.0f));
    node.applyForce(Vec2(3.0f, 4.0f));

    CHECK(node.forceAccumulator.x == 4.0f);
    CHECK(node.forceAccumulator.y == 6.0f);
}

TEST_CASE("Node - applyForce is ignored on fixed node") {
    Node node(Vec2(0.0f, 0.0f), 1.0f, true);

    node.applyForce(Vec2(5.0f, 9.0f));

    CHECK(node.forceAccumulator.x == 0.0f);
    CHECK(node.forceAccumulator.y == 0.0f);
}

TEST_CASE("Node - update performs Symplectic Euler step and damping") {
    Node node(Vec2(0.0f, 0.0f), 2.0f, false);
    node.applyForce(Vec2(2.0f, 4.0f)); // acceleration = (1, 2)

    node.update();

    const float expectedVelXBeforeDamping = 1.0f * Physics::DT;
    const float expectedVelYBeforeDamping = 2.0f * Physics::DT;
    const float expectedPosX = expectedVelXBeforeDamping * Physics::DT;
    const float expectedPosY = expectedVelYBeforeDamping * Physics::DT;
    const float expectedVelXFinal = expectedVelXBeforeDamping * Physics::DAMPING;
    const float expectedVelYFinal = expectedVelYBeforeDamping * Physics::DAMPING;

    CHECK(node.position.x == doctest::Approx(expectedPosX));
    CHECK(node.position.y == doctest::Approx(expectedPosY));
    CHECK(node.velocity.x == doctest::Approx(expectedVelXFinal));
    CHECK(node.velocity.y == doctest::Approx(expectedVelYFinal));
}

TEST_CASE("Node - update clears force accumulator") {
    Node node(Vec2(0.0f, 0.0f), 1.0f, false);
    node.applyForce(Vec2(10.0f, -4.0f));

    node.update();

    CHECK(node.forceAccumulator.x == 0.0f);
    CHECK(node.forceAccumulator.y == 0.0f);
}

TEST_CASE("Node - fixed node does not move or change velocity on update") {
    Node node(Vec2(7.0f, -3.0f), 1.0f, true);
    node.velocity = Vec2(1.0f, 2.0f);
    node.applyForce(Vec2(100.0f, 100.0f)); // ignored for fixed nodes

    node.update();

    CHECK(node.position.x == 7.0f);
    CHECK(node.position.y == -3.0f);
    CHECK(node.velocity.x == 1.0f);
    CHECK(node.velocity.y == 2.0f);
    CHECK(node.forceAccumulator.x == 0.0f);
    CHECK(node.forceAccumulator.y == 0.0f);
}

TEST_CASE("Node - update throws when mass is zero") {
    Node node(Vec2(0.0f, 0.0f), 0.0f, false);
    node.applyForce(Vec2(1.0f, 1.0f));

    CHECK_THROWS_AS(node.update(), std::runtime_error);
}
