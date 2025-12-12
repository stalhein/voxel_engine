#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

class Player
{
public:
    glm::vec3 position, front, frontXY, up, right, worldUp, size, velocity;
    float yaw = -90.f, pitch = 0.f, speed = 20.f, sensitivity = 0.1f;

    Player(glm::vec3 pos);

    void update(GLFWwindow* window, float xoffset, float yoffset, float deltaTime);
    
    glm::mat4 getViewMatrix();
private:
    bool falling = true;
    void applyCollisions(float dt);
    void updateVectors();
};