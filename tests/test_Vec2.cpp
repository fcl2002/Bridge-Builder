#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../classes/Vec2.h"

// -----------------------------------------------------------
// Construction
// -----------------------------------------------------------

TEST_CASE("Vec2 - default constructor initializes to zero") {
    Vec2 v;
    CHECK(v.x == 0.0f);
    CHECK(v.y == 0.0f);
}

TEST_CASE("Vec2 - parameterized constructor") {
    Vec2 v(3.0f, 4.0f);
    CHECK(v.x == 3.0f);
    CHECK(v.y == 4.0f);
}

// -----------------------------------------------------------
// Arithmetic operations
// -----------------------------------------------------------

TEST_CASE("Vec2 - arithmetic operations") {
    Vec2 a(1.0f, 2.0f);
    Vec2 b(3.0f, 4.0f);

    SUBCASE("addition") {
        Vec2 r = a + b;
        CHECK(r.x == 4.0f);
        CHECK(r.y == 6.0f);
    }

    SUBCASE("subtraction") {
        Vec2 r = a - b;
        CHECK(r.x == -2.0f);
        CHECK(r.y == -2.0f);
    }

    SUBCASE("scalar multiplication (right)") {
        Vec2 r = a * 2.0f;
        CHECK(r.x == 2.0f);
        CHECK(r.y == 4.0f);
    }

    SUBCASE("scalar multiplication (left) - free operator") {
        Vec2 r = 2.0f * a;
        CHECK(r.x == 2.0f);
        CHECK(r.y == 4.0f);
    }

    SUBCASE("negation") {
        Vec2 r = -a;
        CHECK(r.x == -1.0f);
        CHECK(r.y == -2.0f);
    }
}

TEST_CASE("Vec2 - division by zero throws exception") {
    Vec2 v(1.0f, 1.0f);
    CHECK_THROWS_AS(v / 0.0f, std::runtime_error);
}

// -----------------------------------------------------------
// Physical operations
// -----------------------------------------------------------

TEST_CASE("Vec2 - magnitude") {
    SUBCASE("3-4-5 right triangle") {
        Vec2 v(3.0f, 4.0f);
        CHECK(v.magnitude() == doctest::Approx(5.0f));
    }

    SUBCASE("zero vector") {
        Vec2 v(0.0f, 0.0f);
        CHECK(v.magnitude() == doctest::Approx(0.0f));
    }
}

TEST_CASE("Vec2 - normalization") {
    SUBCASE("unit vector has magnitude 1") {
        Vec2 v(3.0f, 4.0f);
        Vec2 n = v.normalized();
        CHECK(n.magnitude() == doctest::Approx(1.0f));
        CHECK(n.x == doctest::Approx(0.6f));
        CHECK(n.y == doctest::Approx(0.8f));
    }

    SUBCASE("zero vector throws exception") {
        Vec2 v(0.0f, 0.0f);
        CHECK_THROWS_AS(v.normalized(), std::runtime_error);
    }
}

TEST_CASE("Vec2 - dot product") {
    SUBCASE("perpendicular vectors yield zero") {
        Vec2 a(1.0f, 0.0f);
        Vec2 b(0.0f, 1.0f);
        CHECK(a.dot(b) == doctest::Approx(0.0f));
    }

    SUBCASE("parallel vectors yield product of magnitudes") {
        Vec2 a(2.0f, 0.0f);
        Vec2 b(3.0f, 0.0f);
        CHECK(a.dot(b) == doctest::Approx(6.0f));
    }
}

TEST_CASE("Vec2 - zero() resets components") {
    Vec2 v(5.0f, 9.0f);
    v.zero();
    CHECK(v.x == 0.0f);
    CHECK(v.y == 0.0f);
}

// -----------------------------------------------------------
// Physical context - Symplectic Euler integration step
// -----------------------------------------------------------

TEST_CASE("Vec2 - Symplectic Euler integration step") {
    Vec2 position(0.0f, 10.0f);
    Vec2 velocity(1.0f, 0.0f);
    Vec2 force(0.0f, -9.8f);
    float mass = 1.0f;
    float dt   = 0.016f;

    velocity += (force / mass) * dt;
    position += velocity * dt;

    CHECK(position.x > 0.0f);  // moved right
    CHECK(position.y < 10.0f); // fell slightly
}
