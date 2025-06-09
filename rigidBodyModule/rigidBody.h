#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>
#include <mutex>
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>
#include "nmpcController.h"
#include "constants.h"


// // Constants
// const float M = 1.0f;     // Mass of quadcopter (kg)
// const float J = 0.01f;    // Moment of inertia (kg·m²)
// const float G = 9.81f;    // Gravity (m/s²)
// const float D = 0.2f;     // Propeller arm length (m)
// const float LAMBDA = 0.0f;// Ground height (m)
// const float K_PENALTY = 1000.0f; // Penalty force constant for ground constraint
// const float KAPPA = 0.01f; // Torque constant for yaw (adjust as needed)

// States
struct RigidBodyState {
    glm::vec3 position;       // x, y, z
    float phi;                // φ (roll)
    float theta;              // θ (pitch)
    float psi;                // ψ (yaw)
    glm::vec3 velocity;       // ẋ, ẏ, ż
    float phi_dot;            // φ̇ (roll rate)
    float theta_dot;          // θ̇ (pitch rate)
    float psi_dot;            // ψ̇ (yaw rate)
    glm::vec3 rotationAxis;   // For visualization
    float angleDeg;           // For visualization
};

class RigidBodySimulator {
public:
    RigidBodySimulator();
    ~RigidBodySimulator();

    void start();
    void stop();
    RigidBodyState getLatestState();
    std::vector<float> rk4Integrate(const std::vector<float>& state, const float F[4], float dt);
    std::vector<float> derivatives(const std::vector<float>& s, const float F[4]);

private:
    void simulationLoop();

    std::thread simThread;
    std::mutex stateMutex;
    RigidBodyState currentState;
    std::atomic<bool> running;
    MPCController* controller; // MPC controller for computing control inputs
};

// Draws a pixel at the origin
void drawPosition();

// Draws a box at the specified position and rotation
void drawBoxAt(const glm::vec3& position, float angleDeg, const glm::vec3& axis);

#endif