#include "Object.h"

Object::Object(float m, const std::vector<float>& pos, const std::vector<float>& vel)
    : mass(m), position(pos), velocity(vel) {}

// Position getters and setters
void Object::setPosition(const std::vector<float>& pos) {
    position = pos;
}

void Object::setPositionX(float x) {
    position[0] = x;
}

void Object::setPositionY(float y) {
    position[1] = y;
}

float Object::getPositionX() const {
    return position[0];
}

float Object::getPositionY() const {
    return position[1];
}

// Velocity getters and setters
void Object::setVelocity(const std::vector<float>& v) {
    velocity = v;
}

void Object::setVelocityX(float x) {
    velocity[0] = x;
}

void Object::setVelocityY(float y) {
    velocity[1] = y;
}

float Object::getVelocityX() const {
    return velocity[0];
}

float Object::getVelocityY() const {
    return velocity[1];
}

// Mass
float Object::getMass() const {
    return mass;
}

float Object::setMass(float m) {
    mass = m;
    return mass;
}
