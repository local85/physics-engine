#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

class Object {
public:
    Object(float m, const std::vector<float>& pos, const std::vector<float>& vel);

    virtual void draw() = 0;
    virtual void render() = 0;
    virtual void update(float deltaTime) = 0;
    virtual bool isCircle() const = 0;
    virtual ~Object() = default;

protected:
    // position getters and setters
    void setPosition(const std::vector<float>& pos);
    void setPositionX(float x);
    void setPositionY(float y);
    float getPositionX() const;
    float getPositionY() const;

    // velocity getters and setters
    void setVelocity(const std::vector<float>& v);
    void setVelocityX(float x);
    void setVelocityY(float y);
    float getVelocityX() const;
    float getVelocityY() const;

    // mass
    float getMass() const;
    float setMass(float m);

private:
    float mass;
    // position is the center of the object
    std::vector<float> position;
    std::vector<float> velocity;
};

#endif // OBJECT_H
