#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
using namespace std;
using namespace glm;

// Window dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

// gravtity constant
const float GRAVITY = 9.81f;

// Callback for handling window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Process input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// physics object base class
class Object {
    public:
        Object(float m) : mass(m) {}

    protected:
        // position getters and setters
        void setPosition(const vector<float>& pos) {
            position = pos;
        }
        void setPositionX(float x) {
            position[0] = x;
        }
        void setPositionY(float y) {
            position[1] = y;
        }
        float getPositionX() const {
            return position[0];
        }
        float getPositionY() const {
            return position[1];
        }

        // velocity getters and setters
        void setVelocity(const vector<float>& v) {
            velocity = v;
        }
        void setVelocityX(float x) {
            velocity[0] = x;
        }
        void setVelocityY(float y) {
            velocity[1] = y;
        }
        float getVelocityX() const {
            return velocity[0];
        }
        float getVelocityY() const {
            return velocity[1];
        }

    private:
        float mass;
        // position is the center of the object
        vector<float> position = {1.0f, 1.0f};
        vector<float> velocity = {0.0f, 0.0f};
};

// circle class derived from Object
class Circle : public Object {
    public:
        Circle(float m) : Object(m) {}
        // draw the circle using triangle fan + trig
        void draw() {
            glBegin(GL_TRIANGLE_FAN);
            glVertex2d(getPositionX(), getPositionY());

            for (size_t i = 0; i <= resolution; i++) {
                float angle = 2.0f * M_PI * (static_cast<float>(i) / resolution);
                float x = getPositionX() + radius * cos(angle);
                float y = getPositionY() + radius * sin(angle);
                glVertex2d(x, y);
            }

            glEnd();
        }

        // render the circle
        void render() {
            // draw circle
            draw();

            // update position + velocity
            setPositionX(getPositionX() + getVelocityX());
            setPositionY(getPositionY() + getVelocityY());
            setVelocityY(getVelocityY() - GRAVITY * 0.0000005f);

        }

    private:
        float radius = 0.10f;
        int resolution = 100;
};

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

    // Set up viewport
    glViewport(0, 0, WIDTH, HEIGHT);

    // Set up orthographic projection matching window aspect ratio
    float aspectRatio = (float)WIDTH / (float)HEIGHT;  // 800/600 = 1.333 (4:3)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);  // x: [-1.33, 1.33], y: [-1, 1]
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // make a circle
    Circle c(1.0f);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        c.render();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}
