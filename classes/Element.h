//Element.h
#pragma once
#include "Vec2.h"

class Node; //Forward declaration to avoid circular dependency

/**
 * @brief Abstract class for structural elements of the bridge
 * Represents an element of the truss
 * Transmits axial forces between two nodes
 * Neglects bending and shear forces for simplicity
 */

class Element{
    public:
        Node* nodeA;
        Node* nodeB;
        float restLength;
        bool isBroken;

        /**
         * @param nodeA Pointer to the first node of the element
         * @param nodeB Pointer to the second node of the element
         * @param restLength The natural length of the element (unstressed)
         */
        Element(Node* nodeA, Node* nodeB, float restLength);

        virtual ~Element() = default;

        /**
         * @brief Applies the axial forces to the connected nodes
         * Implemented by derived classes based on the material
         */
        virtual void applyForces() = 0;

        /**
         * @brief Checks if the element has exceeded its breaking threshold
         * Implemented by derived classes based on the material
         */
        virtual void checkBreaking() = 0;

        /**
         * @brief Calculates the current length of the element
         * L = |posB - posA|
         */
        float currentLength() const;

        /**
         * @brief Calculates the normalized axial tension force
         * σ = (L - restLength) / restLength
         * Positive σ indicates tension, negative σ indicates compression
         */
        float axialStrain() const;
};