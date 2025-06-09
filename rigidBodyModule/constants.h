#ifndef CONSTANTS_H
#define CONSTANTS_H
// Constants
const float M = 0.5f;     // Mass of quadcopter (kg)
const float J = 0.001f;    // Moment of inertia (kg·m²)
const float G = 9.81f;    // Gravity (m/s²)
const float D = 0.4f;     // Propeller arm length (m)
const float LAMBDA = 0.0f;// Ground height (m)
const float K_PENALTY = 1000.0f; // Penalty force constant for ground constraint
const float KAPPA = 0.01f; // Torque constant for yaw (adjust as needed)

// Global variables for box dimensions
const float boxHeight = 0.2f;
const float boxWidth = 0.2f;
const float boxLength = 0.2f;

#endif