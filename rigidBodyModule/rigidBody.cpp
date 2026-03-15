#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "rigidBody.h"
#include "nmpcController.h"
#include <chrono>
#include <cmath>
#include <vector>
#include <fstream>


void drawPosition() {
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
}

// void drawBoxAt(const glm::vec3& position, float angleDeg, const glm::vec3& axis) {
//     glPushMatrix();
//     glTranslatef(position.x, position.y, position.z);
//     glRotatef(angleDeg, axis.x, axis.y, axis.z);
//     glColor3f(1.0f, 0.0f, 0.0f);
//     glBegin(GL_QUADS);
//     // Front face
//     glVertex3f(-boxWidth/2, -boxHeight/2,  boxLength/2);
//     glVertex3f( boxWidth/2, -boxHeight/2,  boxLength/2);
//     glVertex3f( boxWidth/2,  boxHeight/2,  boxLength/2);
//     glVertex3f(-boxWidth/2,  boxHeight/2,  boxLength/2);
//     // Back face
//     glVertex3f(-boxWidth/2, -boxHeight/2, -boxLength/2);
//     glVertex3f(-boxWidth/2,  boxHeight/2, -boxLength/2);
//     glVertex3f( boxWidth/2,  boxHeight/2, -boxLength/2);
//     glVertex3f( boxWidth/2, -boxHeight/2, -boxLength/2);
//     // Left face
//     glVertex3f(-boxWidth/2, -boxHeight/2, -boxLength/2);
//     glVertex3f(-boxWidth/2, -boxHeight/2,  boxLength/2);
//     glVertex3f(-boxWidth/2,  boxHeight/2,  boxLength/2);
//     glVertex3f(-boxWidth/2,  boxHeight/2, -boxLength/2);
//     // Right face
//     glVertex3f( boxWidth/2, -boxHeight/2, -boxLength/2);
//     glVertex3f( boxWidth/2,  boxHeight/2, -boxLength/2);
//     glVertex3f( boxWidth/2,  boxHeight/2,  boxLength/2);
//     glVertex3f( boxWidth/2, -boxHeight/2,  boxLength/2);
//     // Top face
//     glVertex3f(-boxWidth/2,  boxHeight/2, -boxLength/2);
//     glVertex3f(-boxWidth/2,  boxHeight/2,  boxLength/2);
//     glVertex3f( boxWidth/2,  boxHeight/2,  boxLength/2);
//     glVertex3f( boxWidth/2,  boxHeight/2, -boxLength/2);
//     // Bottom face
//     glVertex3f(-boxWidth/2, -boxHeight/2, -boxLength/2);
//     glVertex3f( boxWidth/2, -boxHeight/2, -boxLength/2);
//     glVertex3f( boxWidth/2, -boxHeight/2,  boxLength/2);
//     glVertex3f(-boxWidth/2, -boxHeight/2,  boxLength/2);
//     glEnd();
//     glPopMatrix();
// }
void drawBoxAt(const glm::vec3& position, float angleDeg, const glm::vec3& axis) {
    // Draw main box
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(angleDeg, axis.x, axis.y, axis.z);

    glColor3f(1.0f, 0.0f, 0.0f); // Red main box
    glBegin(GL_QUADS);
    // Front face
    glVertex3f(-boxWidth/2, -boxHeight/2,  boxLength/2);
    glVertex3f( boxWidth/2, -boxHeight/2,  boxLength/2);
    glVertex3f( boxWidth/2,  boxHeight/2,  boxLength/2);
    glVertex3f(-boxWidth/2,  boxHeight/2,  boxLength/2);
    // Back face
    glVertex3f(-boxWidth/2, -boxHeight/2, -boxLength/2);
    glVertex3f(-boxWidth/2,  boxHeight/2, -boxLength/2);
    glVertex3f( boxWidth/2,  boxHeight/2, -boxLength/2);
    glVertex3f( boxWidth/2, -boxHeight/2, -boxLength/2);
    // Left face
    glVertex3f(-boxWidth/2, -boxHeight/2, -boxLength/2);
    glVertex3f(-boxWidth/2, -boxHeight/2,  boxLength/2);
    glVertex3f(-boxWidth/2,  boxHeight/2,  boxLength/2);
    glVertex3f(-boxWidth/2,  boxHeight/2, -boxLength/2);
    // Right face
    glVertex3f( boxWidth/2, -boxHeight/2, -boxLength/2);
    glVertex3f( boxWidth/2,  boxHeight/2, -boxLength/2);
    glVertex3f( boxWidth/2,  boxHeight/2,  boxLength/2);
    glVertex3f( boxWidth/2, -boxHeight/2,  boxLength/2);
    // Top face
    glVertex3f(-boxWidth/2,  boxHeight/2, -boxLength/2);
    glVertex3f(-boxWidth/2,  boxHeight/2,  boxLength/2);
    glVertex3f( boxWidth/2,  boxHeight/2,  boxLength/2);
    glVertex3f( boxWidth/2,  boxHeight/2, -boxLength/2);
    // Bottom face
    glVertex3f(-boxWidth/2, -boxHeight/2, -boxLength/2);
    glVertex3f( boxWidth/2, -boxHeight/2, -boxLength/2);
    glVertex3f( boxWidth/2, -boxHeight/2,  boxLength/2);
    glVertex3f(-boxWidth/2, -boxHeight/2,  boxLength/2);
    glEnd();

    // Parameters for propeller boxes
    float propWidth  = boxWidth / 3.0f;
    float propHeight = boxHeight / 3.0f;
    float propLength = boxLength / 8.0f;

    // Offsets for propellers in XY plane
    glm::vec3 offsets[] = {
        { D, 0.0f, 0.0f },  // +X
        {-D, 0.0f, 0.0f },  // -X
        { 0.0f, D, 0.0f },  // +Y
        { 0.0f,-D, 0.0f }   // -Y
    };

    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        glTranslatef(offsets[i].x, offsets[i].y, offsets[i].z);
        
        // Black faces
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        // Front face
        glVertex3f(-propWidth/2, -propHeight/2,  propLength/2);
        glVertex3f( propWidth/2, -propHeight/2,  propLength/2);
        glVertex3f( propWidth/2,  propHeight/2,  propLength/2);
        glVertex3f(-propWidth/2,  propHeight/2,  propLength/2);
        // Back face
        glVertex3f(-propWidth/2, -propHeight/2, -propLength/2);
        glVertex3f(-propWidth/2,  propHeight/2, -propLength/2);
        glVertex3f( propWidth/2,  propHeight/2, -propLength/2);
        glVertex3f( propWidth/2, -propHeight/2, -propLength/2);
        // Left face
        glVertex3f(-propWidth/2, -propHeight/2, -propLength/2);
        glVertex3f(-propWidth/2, -propHeight/2,  propLength/2);
        glVertex3f(-propWidth/2,  propHeight/2,  propLength/2);
        glVertex3f(-propWidth/2,  propHeight/2, -propLength/2);
        // Right face
        glVertex3f( propWidth/2, -propHeight/2, -propLength/2);
        glVertex3f( propWidth/2,  propHeight/2, -propLength/2);
        glVertex3f( propWidth/2,  propHeight/2,  propLength/2);
        glVertex3f( propWidth/2, -propHeight/2,  propLength/2);
        // Top face
        glVertex3f(-propWidth/2,  propHeight/2, -propLength/2);
        glVertex3f(-propWidth/2,  propHeight/2,  propLength/2);
        glVertex3f( propWidth/2,  propHeight/2,  propLength/2);
        glVertex3f( propWidth/2,  propHeight/2, -propLength/2);
        // Bottom face
        glVertex3f(-propWidth/2, -propHeight/2, -propLength/2);
        glVertex3f( propWidth/2, -propHeight/2, -propLength/2);
        glVertex3f( propWidth/2, -propHeight/2,  propLength/2);
        glVertex3f(-propWidth/2, -propHeight/2,  propLength/2);
        glEnd();

        // Yellow edges
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);
        // 8 edges (drawn manually)
        float x = propWidth/2, y = propHeight/2, z = propLength/2;
        glm::vec3 corners[8] = {
            {-x, -y, -z}, { x, -y, -z}, { x,  y, -z}, {-x,  y, -z},
            {-x, -y,  z}, { x, -y,  z}, { x,  y,  z}, {-x,  y,  z}
        };
        // Edges between corners
        int edges[12][2] = {
            {0,1},{1,2},{2,3},{3,0}, // bottom
            {4,5},{5,6},{6,7},{7,4}, // top
            {0,4},{1,5},{2,6},{3,7}  // sides
        };
        for (int e = 0; e < 12; ++e) {
            glVertex3fv(glm::value_ptr(corners[edges[e][0]]));
            glVertex3fv(glm::value_ptr(corners[edges[e][1]]));
        }
        glEnd();

        glPopMatrix();
    }

    glPopMatrix(); // End main box transform
}





RigidBodySimulator::RigidBodySimulator() {
    // Initialize state
    currentState.position = glm::vec3(0.0f, 0.0f, 0.0f);
    currentState.phi = 0.0f;
    currentState.theta = 0.0f;
    currentState.psi = 0.0f;
    currentState.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    currentState.phi_dot = 0.0f;
    currentState.theta_dot = 0.0f;
    currentState.psi_dot = 0.0f;
    currentState.rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
    currentState.angleDeg = 0.0f;
    running = false;
    controller = new MPCController(0.01f, 10);
}

RigidBodySimulator::~RigidBodySimulator() {
    stop();
    delete controller;
}

void RigidBodySimulator::start() {
    if (!running) {
        running = true;
        simThread = std::thread(&RigidBodySimulator::simulationLoop, this);
    }
}

void RigidBodySimulator::stop() {
    running = false;
    if (simThread.joinable()) {
        simThread.join();
    }
}

RigidBodyState RigidBodySimulator::getLatestState() {
    std::lock_guard<std::mutex> lock(stateMutex);
    return currentState;
}

std::vector<float> RigidBodySimulator::rk4Integrate(const std::vector<float>& state, const float F[4], float dt) {
    // State vector: [x, y, z, phi, theta, psi, x_dot, y_dot, z_dot, phi_dot, theta_dot, psi_dot]
    std::vector<float> k1 = derivatives(state, F);
    std::vector<float> state2 = state, state3 = state, state4 = state;
    
    // k2: use k1 to advance state by dt/2
    for (size_t i = 0; i < state.size(); ++i) {
        state2[i] = state[i] + 0.5f * dt * k1[i];
    }
    std::vector<float> k2 = derivatives(state2, F);
    
    // k3: use k2 to advance state by dt/2
    for (size_t i = 0; i < state.size(); ++i) {
        state3[i] = state[i] + 0.5f * dt * k2[i];
    }
    std::vector<float> k3 = derivatives(state3, F);
    
    // k4: use k3 to advance state by dt
    for (size_t i = 0; i < state.size(); ++i) {
        state4[i] = state[i] + dt * k3[i];
    }
    std::vector<float> k4 = derivatives(state4, F);
    
    // Combine using RK4 formula
    std::vector<float> newState(state.size());
    for (size_t i = 0; i < state.size(); ++i) {
        newState[i] = state[i] + (dt / 6.0f) * (k1[i] + 2.0f * k2[i] + 2.0f * k3[i] + k4[i]);
    }
    return newState;
}

std::vector<float> RigidBodySimulator::derivatives(const std::vector<float>& s, const float F[4]) {
    std::vector<float> d(s.size(), 0.0f);
    
    float x = s[0], y = s[1], z = s[2];
    float phi = s[3], theta = s[4], psi = s[5];
    float x_dot = s[6], y_dot = s[7], z_dot = s[8];
    float phi_dot = s[9], theta_dot = s[10], psi_dot = s[11];
    
    float F_sum = F[0] + F[1] + F[2] + F[3];
    
    d[0] = x_dot; d[1] = y_dot; d[2] = z_dot;
    d[3] = phi_dot; d[4] = theta_dot; d[5] = psi_dot;
    
    d[6] = (F_sum / M) * (cosf(phi) * sinf(theta) * cosf(psi) + sinf(phi) * sinf(psi));
    d[7] = (F_sum / M) * (cosf(phi) * sinf(theta) * sinf(psi) - sinf(phi) * cosf(psi));
    d[8] = (F_sum / M) * (cosf(phi) * cosf(theta)) - G;
    
    if (z < LAMBDA) {
        float penalty = K_PENALTY * (LAMBDA - z);
        d[8] += penalty / M;
    }
    
    // MATCHED MOMENT MAPPING
    d[9]  = (D / J) * (F[3] - F[2]);                    
    d[10] = (D / J) * (F[0] - F[1]);                    
    d[11] = (KAPPA / J) * (F[0] + F[1] - F[2] - F[3]); 
    
    return d;
}

void RigidBodySimulator::simulationLoop() {
    float dt = 0.01f; // Time step (s)
    float F[4] = {2.4525f, 2.4525f, 2.4525f, 2.4525f}; // Default forces to hover (mg/4)
    // float F[4] = {2.6525f, 2.4525f, 2.4525f, 2.4525f}; // To roll funnily away from the screen haha
    // float F[4] = {3.4525f, 3.4525f, 3.4525f, 3.4525f};

    // Open a file for logging
    std::ofstream logFile("sim_data.csv");
    logFile << "time,x,y,z,ref_x,ref_y,ref_z\n"; // Header
    float currentTime = 0.0f;

    // 1. Initialize the time point
    auto next_tick = std::chrono::steady_clock::now();

    // used to swap between two references
    int swapReferenceCounter = 0;
    int right = 0;

    // --- CIRCLE PARAMETERS ---
    float radius = 2.0f;       // Radius in meters
    float speed = 1.0f;        // Linear speed in m/s
    float altitude = 2.0f;     // Fixed height
    // -------------------------

    while (running) {

        // 2. Set the target time for the NEXT iteration
        next_tick += std::chrono::microseconds(static_cast<int>(dt * 1000000));

        // this is used to swap between 2 references
        // swapReferenceCounter += 1;
        // if(swapReferenceCounter == 200){
        //     if(right){
        //         right = false;
        //         swapReferenceCounter = 0;
        //         this->controller->setRef(std::vector<float>{-3.0f, -3.0f, 1.5f});
        //     }else{
        //         this->controller->setRef(std::vector<float>{-3.0f, -3.0f, 1.5f});
        //         right = true;
        //         swapReferenceCounter = 0;
        //     }
        // }

        // code for path following a sircle
        // angle = (speed / radius) * time
        float angle = (speed / radius) * currentTime;
        
        float targetX = radius * cosf(angle);
        float targetY = radius * sinf(angle);

        // Update the NMPC reference
        this->controller->setRef(std::vector<float>{targetX, targetY, altitude});

        // Get current state
        RigidBodyState state = getLatestState();
        
        // Compute control inputs using MPC
        controller->computeControl(state, F);

        // LOG DATA HERE
        logFile << currentTime << "," 
                << state.position.x << "," << state.position.y << "," << state.position.z << ","
                << controller->x_ref[0] << "," << controller->x_ref[1] << "," << controller->x_ref[2] << "\n";

        std::cout << "F: " << F[0] << ", " << F[1] << ", " << F[2] << ", " << F[3] << "     Position:" << state.position[0] << ", " << state.position[1] << ", " << state.position[2] << "      Ref:" << controller->x_ref[0] << ", " << controller->x_ref[1] << ", " << controller->x_ref[2] << std::endl;
        std::cout.flush();
        
        // Prepare state vector for integration
        std::vector<float> stateVec = {
            state.position.x, state.position.y, state.position.z,
            state.phi, state.theta, state.psi,
            state.velocity.x, state.velocity.y, state.velocity.z,
            state.phi_dot, state.theta_dot, state.psi_dot
        };
        
        // Integrate using RK4
        std::vector<float> newState = rk4Integrate(stateVec, F, dt);
        
        // Update state with Fixed visualization
        {
            std::lock_guard<std::mutex> lock(stateMutex);
            currentState.position = glm::vec3(newState[0], newState[1], newState[2]);
            currentState.phi = newState[3];
            currentState.theta = newState[4];
            currentState.psi = newState[5];
            currentState.velocity = glm::vec3(newState[6], newState[7], newState[8]);
            currentState.phi_dot = newState[9];
            currentState.theta_dot = newState[10];
            currentState.psi_dot = newState[11];
            
            // Safety check to prevent NaNs when drone is level
            float angleLen = glm::length(glm::vec3(currentState.phi, currentState.theta, currentState.psi));
            if (angleLen > 1e-6f) {
                currentState.rotationAxis = glm::normalize(glm::vec3(currentState.phi, currentState.theta, currentState.psi));
                currentState.angleDeg = angleLen * 180.0f / 3.14159f;
            } else {
                currentState.rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
                currentState.angleDeg = 0.0f;
            }
        }
        currentTime += dt;
        // 3. Sleep only for the remaining time in the budget
        std::this_thread::sleep_until(next_tick);
    }
    logFile.close();
}




