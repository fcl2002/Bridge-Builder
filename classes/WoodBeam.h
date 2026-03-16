//WoodBeam.h
#pragma once
#include "Element.h"

/**
 * @brief WoodBeam is a concrete implementation of Element representing a wooden beam
 * The axial rigidity is defined by k = E * A / L, where:
 * - E is the Young's modulus of wood (approx. 10 GPa)
 * - A is the cross-sectional area of the beam (assumed constant)
 * - L is the current length of the beam
 * 
 * Wood particularity: it can withstand more compression than tension
 */

 class WoodBeam: public Element{
    public:
        static constexpr float ELASTIC_MODULUS = 10e9f; //Pa
        static constexpr float SECTION_AREA    = 0.01f; //m^2

        float maxTensileStress;
        float maxCompressiveStress;
        float stiffness;
        float colorFactor; //For visual representation (0 = no stress, 1 = breaking point)
        float isRoad;
        float cost;

        /**
         * @param nodeA Pointer to the first node of the beam
         * @param nodeB Pointer to the second node of the beam
         * @param restLength The natural length of the beam (unstressed)
         * @param maxTensileStress Maximum tensile stress the beam can withstand before breaking (Pa)
         * @param maxCompressiveStress Maximum compressive stress the beam can withstand before breaking (Pa)
         * @param isRoad Whether the beam is part of the road surface (affects visual representation and cost)
         * @param cost Cost of the beam (used for budget calculations)
         */
        WoodBeam(Node* nodeA,
                 Node* nodeB,
                 float restLength,
                 float maxTensileStress = 0.05f,
                 float maxCompressiveStress = 0.10f,
                 bool isRoad = false,
                 float cost = 1.0f);

        /**
         * @brief Applies the Hooke's axial forces in both nodes
         * F = k * (L - restLength) * direction
         */
        void applyForces() override;

        /**
         * @brief Checks if the beam has exceeded its breaking threshold
         * If axial strain is positive (tension), compare with maxTensileStress
         * If axial strain is negative (compression), compare with maxCompressiveStress
         */
        void checkBreaking() override;

        /**
         * @brief Updates the colorFactor for visual feedback
         * colorFactor -> [0, 1]:
         * 0.0 = max compression (blue)
         * 0.5 = no stress (green)
         * 1.0 = max tension (red)
         */
        void updateColorFactor();
 };