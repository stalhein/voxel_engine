#include "player.hpp"

Player::Player(glm::vec3 pos) : position(pos), front(glm::vec3(0.f, 0.f, -1.f)), up(glm::vec3(0.f, 1.f, 0.f)), worldUp(glm::vec3(0.f, 1.f, 0.f))
{
    updateVectors();    
}

void Player::update(GLFWwindow* window, float xoffset, float yoffset, float deltaTime)
{
    glm::vec3 inputDir(0.f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) inputDir += frontXY;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) inputDir -= frontXY;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) inputDir -= right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) inputDir += right;
}