//Element.cpp
#include "Element.h"
#include "Node.h"

Element::Element(Node* nodeA, Node* nodeB, float restLength):
    nodeA(nodeA),
    nodeB(nodeB),
    restLength(restLength),
    isBroken(false)
{}

float Element::currentLength() const{
    return (nodeB->position - nodeA->position).magnitude();
}

float Element::axialStrain() const{
    return (currentLength() - restLength) / restLength;
}