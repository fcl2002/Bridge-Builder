//Physics.h
#pragma once

/**
 * @brief Global physics constants of the simulation
 * 
 * Static class
 */
class Physics{
    public:
        static constexpr float GRAVITY        = 9.81f; // m/s² 
        static constexpr float DAMPING        = 0.99f;          
        static constexpr float DT             = 0.016f;         // s, step (60 FPS)

       
        static constexpr float K_STIFFNESS     = 1000.0f; // N/px 
        static constexpr float FORCE_BEAM_MAX  = 2000.0f; // N 
        static constexpr float MASS_PER_LENGTH = 0.00f;   // kg/px 

        Physics() = delete;
};
