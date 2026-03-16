//Vec2.h
#pragma once
#include<cmath>
#include<stdexcept>

/**
* @brief 2D Vector for vectorial physics calculations
*
* Represents R² euclidean space elements, used for position,
* velocity, acceleration and forces in the simulation.
*
*/

struct Vec2{
    float x;
    float y;

    //Constructors
    Vec2(): x(0.0f), y(0.0f) {}
    Vec2(float x, float y): x(x), y(y) {}

    //Arithmetic operators
    Vec2 operator+(const Vec2 &other) const{
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2 &other) const{
        return Vec2(x - other.x, y - other.y);
    }

    //Scalar multiplication: v' = v * s
    Vec2 operator*(float scalar) const{
        return Vec2(x * scalar, y * scalar);
    }

    //Scalar division: v' = v / s
    //Used for normalization and acceleration calculation (a = F/m)
    Vec2 operator/(float  scalar) const{
        if(scalar == 0.0f) throw std::runtime_error("Vec2: division by zero");
        return Vec2(x / scalar, y / scalar);
    }

    //Negation operator: v' = -v
    Vec2 operator-() const{
        return Vec2(-x, -y);
    }

    //Compound assignment operators
    Vec2& operator+=(const Vec2& other){
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& other){
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2& operator*=(float scalar){
        x *= scalar;
        y *= scalar;
        return *this;
    }

    //Physical vector operations    
    /**
     * @brief Dot product: u · v = u.x * v*x + u.y * v.y 
     * Used for projection of forces over a direction
     */
    float dot(const Vec2 &other) const{
        return x * other.x + y * other.y;
    }

    /**
     * @brief Magnitude : |v| = sqrt(v.x² + v.y²)
     * Represents the length of the vector
     */
    float magnitude() const{
        return std::sqrt(x * x + y * y);
    }

    /**
     * @brief Squared magnitude: |v|² = v.x² + v.y²
     * Used for performance optimization when comparing vector lengths
     */
    float magnitudeSquared() const{
        return x * x + y * y;
    }

    /**
     * @brief Normalization: v' = v / |v|
     * Converts the vector to a unit vector (length of 1) in the same direction
     */
    Vec2 normalized() const{
        float mag = magnitude();
        if(mag == 0.0f){
            throw std::runtime_error("Vec2: cannot normalize zero vector");
        }
        return *this / mag;
    }

    /**
     * @brief Reset vector to zero: v = (0, 0)
     * Used to clear forces or velocities when needed
     */
    void zero(){
        x = 0.0f;
        y = 0.0f;
    }

    bool operator==(const Vec2& other) const{
        return x == other.x && y == other.y;
    }    
};

inline Vec2 operator*(float scalar, const Vec2& v){
    return v * scalar;
}