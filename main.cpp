#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "rigidBody.h"


int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1600, 1200, "Simulation", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0); // FOV, aspect, near, far

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Set the viewing direction: looking from the x-direction with y to the right and z upwards
    gluLookAt(5.0, 0.0, 2.0,   // Eye position (along the x-axis)
              0.0, 0.0, 2.0,   // Look at origin
              0.0, 0.0, 1.0);  // Up vector (z-axis upwards)


    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }
    //Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

    // Set clear color to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Spawn simulation tread of rigid body
    RigidBodySimulator simulator;
    simulator.start();

    while (!glfwWindowShouldClose(window)) {
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Draw ground
        glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 0.0f); // Green color
        glVertex3f(-10.0f, 0.0f, -10.0f);
        glVertex3f(10.0f, 0.0f, -10.0f);
        glVertex3f(10.0f, 0.0f, 10.0f);
        glVertex3f(-10.0f, 0.0f, 10.0f);
        glEnd();



        // Drawing happens here...
        RigidBodyState state = simulator.getLatestState();
        drawBoxAt(state.position, state.angleDeg, state.rotationAxis);

        //Draw the two reference points
        // Draw first box at {0.0f, 0.0f, 1.5f}
        //drawBoxAt(glm::vec3(0.0f, 0.0f, 1.5f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        // Draw second box at {-3.0f, -3.0f, 1.3f}
        //drawBoxAt(glm::vec3(-3.0f, -3.0f, 1.3f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    simulator.stop();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
