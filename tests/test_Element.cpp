#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../classes/Element.h"
#include "../classes/Node.h"

class DummyElement : public Element {
  public:
    DummyElement(Node* nodeA, Node* nodeB, float restLength)
        : Element(nodeA, nodeB, restLength) {}

    void applyForces() override {}
    void checkBreaking() override {}
};

TEST_CASE("Element - currentLength computes node distance") {
    Node nodeA(Vec2(0.0f, 0.0f));
    Node nodeB(Vec2(3.0f, 4.0f));
    DummyElement element(&nodeA, &nodeB, 5.0f);

    CHECK(element.currentLength() == doctest::Approx(5.0f));
}

TEST_CASE("Element - axialStrain is positive under tension") {
    Node nodeA(Vec2(0.0f, 0.0f));
    Node nodeB(Vec2(5.0f, 0.0f));
    DummyElement element(&nodeA, &nodeB, 4.0f);

    CHECK(element.axialStrain() == doctest::Approx(0.25f));
}

TEST_CASE("Element - axialStrain is negative under compression") {
    Node nodeA(Vec2(0.0f, 0.0f));
    Node nodeB(Vec2(3.0f, 0.0f));
    DummyElement element(&nodeA, &nodeB, 4.0f);

    CHECK(element.axialStrain() == doctest::Approx(-0.25f));
}
