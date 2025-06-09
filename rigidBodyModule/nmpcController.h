#ifndef NMPC_CONTROLLER_H
#define NMPC_CONTROLLER_H

#include <vector>
#include "constants.h"
#include <cmath>
#include <algorithm>
#include <Eigen/Dense>
#include <iostream>


struct RigidBodyState;

class MPCController {
public:
    MPCController(float dt, int horizon);
    void computeControl(const RigidBodyState& currentState, float F[4]);
    void setRef(const std::vector<float>& ref);

    std::vector<float> x_ref; // Target reference: [x, y, z]

private:
    float dt;
    int N; // Prediction horizon
};

#endif
