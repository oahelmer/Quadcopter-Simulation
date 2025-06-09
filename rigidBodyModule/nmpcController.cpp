#include "nmpcController.h"
#include "rigidBody.h"

using namespace Eigen;

MPCController::MPCController(float dt, int horizon) : dt(dt), N(horizon) {
    x_ref.resize(3, 0.0f); // Reference for x, y, z
}

void MPCController::setRef(const std::vector<float>& ref) {
    x_ref = ref;
}

void MPCController::computeControl(const RigidBodyState& currentState, float F[4]) {
    constexpr int n_states = 12;
    constexpr int n_inputs = 4;

    // Continuous-time A and B matrices
    Matrix<float, n_states, n_states> A = Matrix<float, n_states, n_states>::Zero();
    Matrix<float, n_states, n_inputs> B = Matrix<float, n_states, n_inputs>::Zero();

    A(0, 1) = 1;
    A(1, 8) = G / M;
    A(2, 3) = 1;
    A(3, 6) = -G / M;
    A(4, 5) = 1;
    A(6, 7) = 1;
    A(8, 9) = 1;
    A(10, 11) = 1;

    B(5, 0) = B(5, 1) = B(5, 2) = B(5, 3) = 1.0f / M;
    B(7, 1) = D / J;
    B(7, 3) = -D / J;
    B(9, 0) = -D / J;
    B(9, 2) = D / J;
    B(11, 0) = -KAPPA / J;
    B(11, 1) = KAPPA / J;
    B(11, 2) = -KAPPA / J;
    B(11, 3) = KAPPA / J;

    // Discretize A, B (Euler)
    Matrix<float, n_states, n_states> Ad = Matrix<float, n_states, n_states>::Identity() + A * dt;
    Matrix<float, n_states, n_inputs> Bd = B * dt;

    // Cost function: Q penalizes position error, R penalizes control effort
    Matrix<float, n_states, n_states> Q = Matrix<float, n_states, n_states>::Zero();
    Q(0, 0) = Q(2, 2) = Q(4, 4) = 500.0f; // position weights
    Q(1, 1) = Q(3, 3) = Q(5, 5) = 100.0f;  // velocity weights
    Q(6, 6) = Q(8, 8) = Q(10, 10) = 50.0f; // φ, θ, ψ
    Q(7, 7) = Q(9, 9) = Q(11, 11) = 5.0f; // φ̇, θ̇, ψ̇

    Matrix<float, n_inputs, n_inputs> R = Matrix<float, n_inputs, n_inputs>::Identity() * 0.01f;

    // Solve Discrete Algebraic Riccati Equation (DARE)
    Matrix<float, n_states, n_states> P = Q; // Initial guess
    Matrix<float, n_states, n_states> P_next;
    constexpr int max_iter = 1000;
    constexpr float tol = 1e-6f;

    for (int i = 0; i < max_iter; ++i) {
        Matrix<float, n_states, n_states> temp = Bd * R.inverse() * Bd.transpose() * P;
        Matrix<float, n_states, n_states> M = Matrix<float, n_states, n_states>::Identity() + temp;
        P_next = Q + Ad.transpose() * P * M.inverse() * Ad;
        
        if ((P_next - P).norm() < tol) {
            P = P_next;
            break;
        }
        P = P_next;
    }

    // Compute feedback gain K
    Matrix<float, n_inputs, n_states> K = (R.inverse() * Bd.transpose() * P * (Matrix<float, n_states, n_states>::Identity() + Bd * R.inverse() * Bd.transpose() * P).inverse() * Ad);

    // Current state
    Vector<float, n_states> x0;
    x0 << currentState.position.x,
          currentState.velocity.x,
          currentState.position.y,
          currentState.velocity.y,
          currentState.position.z,
          currentState.velocity.z,
          currentState.phi,
          currentState.phi_dot,
          currentState.theta,
          currentState.theta_dot,
          currentState.psi,
          currentState.psi_dot;

    // Reference state (only position and angles)
    Vector<float, n_states> x_des = Vector<float, n_states>::Zero();
    x_des(0) = x_ref[0];
    x_des(2) = x_ref[1];
    x_des(4) = x_ref[2];
    x_des(6) = 0.0f; // φ reference
    x_des(8) = 0.0f; // θ reference
    x_des(10) = 0.0f; // ψ reference

    // Compute control input: u = -K * (x - x_des)
    Vector<float, n_inputs> u = -K * (x0 - x_des);

    // Limit forces
    for (int i = 0; i < 4; ++i) {
        u(i) = std::max(0.0f, std::min(u(i), 15.0f));
        F[i] = u(i);
    }
}