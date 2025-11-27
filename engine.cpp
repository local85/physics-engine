#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <chrono>
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

// physics object base class
class Object {
    public:
        Object(float m, const vector<float>& pos, const vector<float>& vel) : 
            mass(m), position(pos), velocity(vel) {}

        virtual void draw() = 0;
        virtual void render() = 0;
        virtual void update(float deltaTime) = 0;


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
        vector<float> position;
        vector<float> velocity;
};

// circle class derived from Object
class Circle : public Object {
    public:
        Circle(float m, const vector<float>& pos, const vector<float>& vel) : Object(m, pos, vel) {}
        // draw the circle using triangle fan + trig
        void draw() override {
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
        void render() override {
            // draw circle
            draw();
        }

        // update physics based on delta time
        void update(float deltaTime) override{
            // update position based on velocity
            setPositionX(getPositionX() + getVelocityX() * deltaTime);
            setPositionY(getPositionY() + getVelocityY() * deltaTime);

            // apply gravity acceleration to velocity
            setVelocityY(getVelocityY() - GRAVITY * deltaTime);

            // wall collision detection
            // x direction
            if (getPositionX() + radius < -1.125 || 
                getPositionX() + radius > 1.25) {
                    setPositionX(getPositionX() + (getPositionX() * -0.05));
                    setVelocityX(getVelocityX() * -0.95);
                }
            
            // y direction
            if (getPositionY() + radius < -0.875 || 
                getPositionY() + radius > 0.75) {
                    setPositionY(getPositionY() + (getPositionY() * -0.05));
                    setVelocityY(getVelocityY() * -0.95);
                }
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
    objects.push_back(new Circle(1.0f, {0.0f, 1.0f}, {1.0f, 0.0f}));
    objects.push_back(new Circle(1.0f, {0.25f, -0.75f}, {-1.0f, 1.0f}));

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

        for (Object* o : objects) {
            o->render();
            o->update(deltaTime.count());
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}
