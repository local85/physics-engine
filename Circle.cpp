#include "Circle.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

// Gravity constant (defined in engine.cpp)
const float GRAVITY = 9.81f;

Circle::Circle(float m, const std::vector<float>& pos, const std::vector<float>& vel)
    : Object(m, pos, vel) {}

int Circle::getRestitution() const {
    return restitution;
}

void Circle::setRestitution(int r) {
    if (r >= 100) {
        throw std::out_of_range("Restitutions cant be bigger than 99");
    }
    restitution = r;
}

// Draw the circle using triangle fan + trig
void Circle::draw() {
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

// Render the circle
void Circle::render() {
    draw();
}

// Update physics based on delta time
void Circle::update(float deltaTime) {
    // Check if resting on ground before applying gravity
    bool onGround = (getPositionY() - radius <= -0.875 && abs(getVelocityY()) < 0.5);

    // Only apply gravity if not resting on ground
    if (!onGround) {
        setVelocityY(getVelocityY() - GRAVITY * deltaTime);
    }

    // Update position based on velocity
    setPositionX(getPositionX() + getVelocityX() * deltaTime);
    setPositionY(getPositionY() + getVelocityY() * deltaTime);

    // Wall collision detection
    // Left wall
    if (getPositionX() - radius < -1.125) {
        setPositionX(-1.125 + radius);
        setVelocityX(getVelocityX() * -0.80);
    }
    // Right wall
    if (getPositionX() + radius > 1.25) {
        setPositionX(1.25 - radius);
        setVelocityX(getVelocityX() * -0.80);
    }

    // Bottom floor
    if (getPositionY() - radius < -0.875) {
        setPositionY(-0.875 + radius);

        // If bouncing very slowly, stop completely
        if (abs(getVelocityY()) < 0.5) {
            setVelocityY(0);  // Stop bouncing
            setVelocityX(getVelocityX() * 0.95);  // Apply ground friction
        } else {
            setVelocityY(getVelocityY() * -0.80);  // Bounce with energy loss
        }
    }
    // Top ceiling
    if (getPositionY() + radius > 0.75) {
        setPositionY(0.75 - radius);
        setVelocityY(getVelocityY() * -0.80);
    }
}

bool Circle::isCircle() const {
    return true;
}

void Circle::circleCollision(Circle& c) {
    if (this == &c) return;
    float distance = sqrtf(powf(c.getPositionX() - getPositionX(), 2)
        + powf(c.getPositionY() - getPositionY(), 2));

    // Get momentums
    float momentumX = getMass() * getVelocityX();
    float momentumY = getMass() * getVelocityY();
    float momentumX_C = c.getMass() * c.getVelocityX();
    float momentumY_C = c.getMass() * c.getVelocityY();

    float momentum = sqrtf(powf(momentumX, 2) + powf(momentumY, 2));
    float momentum_C = sqrtf(powf(momentumX_C, 2) + powf(momentumY_C, 2));

    // If circles cross
    if (distance <= (radius + c.radius)) {
        // Determine if collision is inelastic / elastic
        if ((momentumX > 2 && momentumX_C > 2) ||
            (momentumY > 2 && momentumY_C > 2)) {
            if (momentum > momentum_C) {
                thisCollidesElastic(c, momentumX, momentumY, momentumX_C, momentumY_C);
            }
            else if (momentum_C > momentum) {
                otherCollidesElastic(c, momentumX, momentumY, momentumX_C, momentumY_C);
            }
            else if (restitution > c.restitution) {
                thisCollidesElastic(c, momentumX, momentumY, momentumX_C, momentumY_C);
            }
            else if (c.restitution > restitution) {
                otherCollidesElastic(c, momentumX, momentumY, momentumX_C, momentumY_C);
            }
            else {
                thisCollidesElastic(c, momentumX, momentumY, momentumX_C, momentumY_C);
            }
        }
        else if ((momentumX <= 2 && momentumX_C <= 2) ||
                (momentumY <= 2 && momentumY_C <= 2))  {
            inelastic(c, momentumX, momentumY, momentumX_C, momentumY_C);
        }
    }
}

// Collision helpers
void Circle::thisCollidesElastic(Circle& c, float momentumX, float momentumY, float momentumX_C, float momentumY_C) {
    setVelocityX(getVelocityX() * (restitution / 100) * (getMass() / c.getMass()));
    setVelocityY(getVelocityY() * (restitution / 100) * (getMass() / c.getMass()));

    c.setVelocityX((momentumX + momentumX_C - (getMass() * getVelocityX())) / c.getMass());
    c.setVelocityY((momentumY + momentumY_C - (getMass() * getVelocityY())) / c.getMass());

    // Calculate collision normal (direction from c to this)
    float dx = getPositionX() - c.getPositionX();
    float dy = getPositionY() - c.getPositionY();
    float distance = sqrtf(dx * dx + dy * dy);
    float normalX = dx / distance;
    float normalY = dy / distance;

    // Calculate overlap amount
    float overlap = (radius + c.radius) - distance;

    // Separate objects along collision normal
    setPositionX(getPositionX() + normalX * overlap * 0.5f);
    setPositionY(getPositionY() + normalY * overlap * 0.5f);
    c.setPositionX(c.getPositionX() - normalX * overlap * 0.5f);
    c.setPositionY(c.getPositionY() - normalY * overlap * 0.5f);
}

void Circle::otherCollidesElastic(Circle& c, float momentumX, float momentumY, float momentumX_C, float momentumY_C) {
    c.setVelocityX(getVelocityX() * (restitution / 100) * (c.getMass() / getMass()));
    c.setVelocityY(getVelocityY() * (restitution / 100) * (c.getMass() / getMass()));

    setVelocityX((momentumX + momentumX_C - (c.getMass() * c.getVelocityX())) / getMass());
    setVelocityY((momentumY + momentumY_C - (c.getMass() * c.getVelocityY())) / getMass());

    // Calculate collision normal (direction from c to this)
    float dx = getPositionX() - c.getPositionX();
    float dy = getPositionY() - c.getPositionY();
    float distance = sqrtf(dx * dx + dy * dy);
    float normalX = dx / distance;
    float normalY = dy / distance;

    // Calculate overlap amount
    float overlap = (radius + c.radius) - distance;

    // Separate objects along collision normal
    setPositionX(getPositionX() + normalX * overlap * 0.5f);
    setPositionY(getPositionY() + normalY * overlap * 0.5f);
    c.setPositionX(c.getPositionX() - normalX * overlap * 0.5f);
    c.setPositionY(c.getPositionY() - normalY * overlap * 0.5f);
}

void Circle::inelastic(Circle& c, float momentumX, float momentumY, float momentumX_C, float momentumY_C) {
    // Calculate collision normal (direction from c to this)
    float dx = getPositionX() - c.getPositionX();
    float dy = getPositionY() - c.getPositionY();
    float distance = sqrtf(dx * dx + dy * dy);
    float normalX = dx / distance;
    float normalY = dy / distance;

    // In a perfectly inelastic collision, both objects move together with the same final velocity
    // Apply conservation of momentum: (m1*v1 + m2*v2) / (m1 + m2)
    float vfinalx = (momentumX + momentumX_C) / (getMass() + c.getMass());
    float vfinaly = (momentumY + momentumY_C) / (getMass() + c.getMass());

    // Apply energy loss (0.95 damping factor for inelastic collision)
    vfinalx *= 0.95f;
    vfinaly *= 0.95f;

    // Give both circles the same base velocity, but add small separation impulse along normal
    // This prevents them from getting stuck together
    float separationSpeed = 0.1f;  // Small impulse to push them apart

    setVelocityX(vfinalx + normalX * separationSpeed);
    setVelocityY(vfinaly + normalY * separationSpeed);
    c.setVelocityX(vfinalx - normalX * separationSpeed);
    c.setVelocityY(vfinaly - normalY * separationSpeed);

    // Calculate overlap amount and separate objects
    float overlap = (radius + c.radius) - distance;

    // Separate objects along collision normal
    setPositionX(getPositionX() + normalX * overlap * 0.5f);
    setPositionY(getPositionY() + normalY * overlap * 0.5f);
    c.setPositionX(c.getPositionX() - normalX * overlap * 0.5f);
    c.setPositionY(c.getPositionY() - normalY * overlap * 0.5f);
}
