#include "nmpcController.h"
#include "rigidBody.h"
#include <iostream>

using namespace Eigen;

MPCController::MPCController(float dt, int horizon) : dt(dt), N(horizon) {
    x_ref.resize(3, 0.0f);
}

void MPCController::setRef(const std::vector<float>& ref) {
    x_ref = ref;
}

// Predict next state using RK4 (Matches Equations 16-21)
std::vector<float> MPCController::dynamics(const std::vector<float>& s, const double F[4]) {
    std::vector<float> d(12, 0.0f);
    float phi = s[3], theta = s[4], psi = s[5];
    float x_dot = s[6], y_dot = s[7], z_dot = s[8];
    float phi_dot = s[9], theta_dot = s[10], psi_dot = s[11];
    float F_sum = F[0] + F[1] + F[2] + F[3];

    d[0] = x_dot; d[1] = y_dot; d[2] = z_dot;
    d[3] = phi_dot; d[4] = theta_dot; d[5] = psi_dot;

    // Linear accelerations (Matches Equations 16-18)
    d[6] = (F_sum / M) * (cosf(phi) * sinf(theta) * cosf(psi) + sinf(phi) * sinf(psi));
    d[7] = (F_sum / M) * (cosf(phi) * sinf(theta) * sinf(psi) - sinf(phi) * cosf(psi));
    d[8] = (F_sum / M) * (cosf(phi) * cosf(theta)) - G;

    // CORRECTED MOMENT MAPPING:
    // Roll (phi) is rotation around X. Uses motors on Y axis (2 and 3).
    // Pitch (theta) is rotation around Y. Uses motors on X axis (0 and 1).
    d[9]  = (D / J) * (F[3] - F[2]);                    // Roll: Left - Right
    d[10] = (D / J) * (F[0] - F[1]);                    // Pitch: Front - Back
    d[11] = (KAPPA / J) * (F[0] + F[1] - F[2] - F[3]); // Yaw: CCW - CW pairs
    return d;
}

// RK4 Implementation
std::vector<float> MPCController::rk4_step(const std::vector<float>& s, const double F[4], float dt) {
    auto k1 = dynamics(s, F);
    std::vector<float> s2(12), s3(12), s4(12);
    for(int i=0; i<12; ++i) s2[i] = s[i] + 0.5f * dt * k1[i];
    auto k2 = dynamics(s2, F);
    for(int i=0; i<12; ++i) s3[i] = s[i] + 0.5f * dt * k2[i];
    auto k3 = dynamics(s3, F);
    for(int i=0; i<12; ++i) s4[i] = s[i] + dt * k3[i];
    auto k4 = dynamics(s4, F);
    
    std::vector<float> next_s(12);
    for(int i=0; i<12; ++i) next_s[i] = s[i] + (dt/6.0f)*(k1[i] + 2*k2[i] + 2*k3[i] + k4[i]);
    return next_s;
}

// Objective Function (Equation 58)
double MPCController::cost_function(const std::vector<double>& u_vec, std::vector<double>& grad, void* data) {
    // Gradient check: COBYLA is gradient-free, so grad should be empty.
    if (!grad.empty()) return 0.0; 

    MPCData* mpcData = static_cast<MPCData*>(data);
    std::vector<float> s = {
        mpcData->currentState->position.x, mpcData->currentState->position.y, mpcData->currentState->position.z,
        mpcData->currentState->phi, mpcData->currentState->theta, mpcData->currentState->psi,
        mpcData->currentState->velocity.x, mpcData->currentState->velocity.y, mpcData->currentState->velocity.z,
        mpcData->currentState->phi_dot, mpcData->currentState->theta_dot, mpcData->currentState->psi_dot
    };

    double cost = 0.0;
    for (int t = 0; t < mpcData->N; ++t) {
        double F_step[4] = {u_vec[t*4], u_vec[t*4+1], u_vec[t*4+2], u_vec[t*4+3]};
        s = rk4_step(s, F_step, mpcData->dt);

        // Penalty for position error
        // Separate Z-weight from X and Y
        float weightXY = 300.0f;
        float weightZ = 300.0f; // Increase this significantly

        cost += weightXY * (pow(s[0] - (*mpcData->x_ref)[0], 2) + 
                            pow(s[1] - (*mpcData->x_ref)[1], 2));
        cost += weightZ * pow(s[2] - (*mpcData->x_ref)[2], 2);

        // ADD DAMPING (Velocity Penalty)
        // This is what stops the oscillations!
        double dampingWeight = 40.0;
        cost += dampingWeight * (pow(s[6], 2) + pow(s[7], 2) + pow(s[8], 2));

        // Penalty for high angles (stability)
        cost += 50.0 * (pow(s[3], 2) + pow(s[4], 2) + pow(s[5], 2));

        // R Matrix: Penalty for control effort (deviation from hover)
        for(int i=0; i<4; ++i) cost += 0.01 * pow(F_step[i] - (M * G / 4.0), 2);
        
        // Ground constraint
        if (s[2] < LAMBDA) cost += 5000.0 * pow(LAMBDA - s[2], 2);
    }
    return cost;
}

void MPCController::computeControl(const RigidBodyState& currentState, float F[4]) {
    // SWITCH TO LN_COBYLA (Local Non-derivative)
    nlopt::opt opt(nlopt::LN_COBYLA, 4 * N); 
    MPCData data = {&currentState, &x_ref, dt, N};
    
    opt.set_min_objective(MPCController::cost_function, &data);
    
    // Constraints
    std::vector<double> lb(4 * N, 0.0);
    std::vector<double> ub(4 * N, 15.0);
    opt.set_lower_bounds(lb);
    opt.set_upper_bounds(ub);

    opt.set_xtol_rel(1e-3);
    opt.set_maxeval(500); // Increased evaluations for better convergence

    // Start from hover
    std::vector<double> u_vec(4 * N, M * G / 4.0);
    double min_f;

    try {
        opt.optimize(u_vec, min_f);
        // Apply only the first control action
        for (int i = 0; i < 4; ++i) F[i] = static_cast<float>(u_vec[i]); 
    } catch (std::exception &e) {
        for (int i = 0; i < 4; ++i) F[i] = M * G / 4.0f;
    }
}