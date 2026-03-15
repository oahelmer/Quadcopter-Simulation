#ifndef NMPC_CONTROLLER_H
#define NMPC_CONTROLLER_H

#include <vector>
#include "constants.h"
#include <nlopt.hpp> // Added for nonlinear optimization
#include <Eigen/Dense>

struct RigidBodyState;

// Data passed to NLopt callback
struct MPCData {
    const RigidBodyState* currentState;
    const std::vector<float>* x_ref;
    float dt;
    int N;
};

class MPCController {
public:
    MPCController(float dt, int horizon);
    void computeControl(const RigidBodyState& currentState, float F[4]);
    void setRef(const std::vector<float>& ref);

    std::vector<float> x_ref;

private:
    float dt;
    int N;

    // Nonlinear dynamics and RK4 for prediction
    static std::vector<float> dynamics(const std::vector<float>& s, const double F[4]);
    static std::vector<float> rk4_step(const std::vector<float>& s, const double F[4], float dt);

    // NLopt objective function
    static double cost_function(const std::vector<double>& u_vec, std::vector<double>& grad, void* data);
    std::vector<double> last_u;
};

#endif