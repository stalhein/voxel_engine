#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "world.hpp"

class Player
{
public:
    glm::vec3 position, front, frontXY, up, right, worldUp, size, velocity;
    float yaw = -90.f, pitch = 0.f, speed = 20.f, sensitivity = 0.1f;

    Player(glm::vec3 pos, World* world);

    void update(GLFWwindow* window, float xoffset, float yoffset, float deltaTime);
    
    glm::mat4 getViewMatrix();
private:
    World* world;

    bool falling = true;
    void applyCollisions(float dt);
    void updateVectors();
    inline glm::vec3 aabbPos()
    {
        return {position.x - size.x/2, position.y - size.y * 0.8, position.z - size.z / 2};
    }

    const float maxSpeed = speed, groundAcceleration = speed * 10.f, airAcceleration = speed * 1.f, groundFriction = 10.f, airDrag = 0.5f, gravity = -30.f, stepHeight = 1.f;

};