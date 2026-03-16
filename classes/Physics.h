//Physics.h
#pragma once

/**
 * @brief Global physics constants of the simulation
 * 
 * Static class
 */
class Physics{
    public:
        static constexpr float GRAVITY = 9.81f; //m/s², acceleration due to gravity
        static constexpr float DAMPING = 0.98f;
        static constexpr float DT      = 0.016f; //s, time step for integration (60 FPS)

        // Prevent instantiation of the Physics class
        Physics() = delete;
};
