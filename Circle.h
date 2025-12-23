#ifndef CIRCLE_H
#define CIRCLE_H

#include "Object.h"
#include <cmath>

class Circle : public Object {
public:
    Circle(float m, const std::vector<float>& pos, const std::vector<float>& vel);

    int getRestitution() const;
    void setRestitution(int r);

    void draw() override;
    void render() override;
    void update(float deltaTime) override;
    bool isCircle() const override;

    void circleCollision(Circle& c);

private:
    // Collision helpers
    void thisCollidesElastic(Circle& c, float momentumX, float momentumY, float momentumX_C, float momentumY_C);
    void otherCollidesElastic(Circle& c, float momentumX, float momentumY, float momentumX_C, float momentumY_C);
    void inelastic(Circle& c, float momentumX, float momentumY, float momentumX_C, float momentumY_C);

    float radius = 0.10f;
    int resolution = 100;
    int restitution = 80;
};

#endif // CIRCLE_H
