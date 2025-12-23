#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <chrono>
#include "Object.h"
#include "Circle.h"

using namespace std;
using namespace glm;

// Window dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

// Gravity constant
const float GRAVITY = 9.81f;

// Callback for handling window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    // Update projection matrix to match new aspect ratio
    float aspectRatio = (float)width / (float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Process input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Physics Engine", nullptr, nullptr);
    if (!window) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Get actual framebuffer size (important for Retina displays)
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

    // Set up viewport
    glViewport(0, 0, fbWidth, fbHeight);

    // Set up orthographic projection matching window aspect ratio
    float aspectRatio = (float)fbWidth / (float)fbHeight;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // initialize Object list
    vector<Object*> objects;

    // make some circles
    objects.push_back(new Circle(15.0f, {-0.5f, 1.0f}, {2.0f, 0.0f}));
    objects.push_back(new Circle(10.0f, {0.25f, -0.75f}, {-2.0f, 5.0f}));
    objects.push_back(new Circle(5.0f, {0.75f, -0.25f}, {6.0f, 3.5f}));

    // Delta time tracking
    auto lastFrame = chrono::high_resolution_clock::now();

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        auto currentFrame = chrono::high_resolution_clock::now();
        chrono::duration<float> deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT);

        // Reset modelview matrix each frame
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        for (size_t i = 0; i < objects.size(); ++i) {
            objects[i]->render();
            if (objects[i]->isCircle()) {
                for (size_t j = i + 1; j < objects.size(); ++j) {
                    if (objects[j]->isCircle()) {
                        static_cast<Circle*>(objects[i])->circleCollision(*static_cast<Circle*>(objects[j]));
                    }
                }
            }
            objects[i]->update(deltaTime.count());
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}
