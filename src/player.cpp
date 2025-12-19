#include "player.hpp"

Player::Player(glm::vec3 pos, World* world) : position(pos), world(world), front(glm::vec3(0.f, 0.f, -1.f)), up(glm::vec3(0.f, 1.f, 0.f)), worldUp(glm::vec3(0.f, 1.f, 0.f))
{
    updateVectors();    
}

void Player::update(GLFWwindow* window, float xoffset, float yoffset, float deltaTime)
{
    glm::vec3 inputDir(0.f);

    /*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) inputDir += frontXY;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) inputDir -= frontXY;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) inputDir -= right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) inputDir += right;

    

    if (glm::length(inputDir) > 0.01f)  inputDir = glm::normalize(inputDir);

    float acceleration = falling ? airAcceleration : groundAcceleration;

    velocity += inputDir * acceleration * deltaTime;

    glm::vec2 horizontal(velocity.x, velocity.z);
    float horizontalLength = glm::length(horizontal);

    if (horizontalLength > maxSpeed) {
        horizontal = (horizontal / horizontalLength) * maxSpeed;
        velocity.x = horizontal.x;
        velocity.z = horizontal.y;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !falling) {
        velocity.y = 14.0f;
    }

    if (!falling) {
        velocity.x -= velocity.x * groundFriction * deltaTime;
        velocity.z -= velocity.z * groundFriction * deltaTime;
    } else {
        velocity.x -= velocity.x * airDrag * deltaTime;
        velocity.z -= velocity.z * airDrag * deltaTime;
    }

    velocity.y += gravity * deltaTime;

    applyCollisions(deltaTime);*/

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += frontXY * (deltaTime * 100);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= frontXY * (deltaTime * 100);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position -= right * (deltaTime * 100);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position += right * (deltaTime * 100);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) position += worldUp * (deltaTime * 100);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) position -= worldUp * (deltaTime * 100);


    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)  pitch = 89.9f;
    if (pitch < -89.0f) pitch = -89.0f;

    xoffset = 0;
    yoffset = 0;
    updateVectors();
}

void Player::applyCollisions(float dt)
{
    glm::vec3 oldPos = position;

    position.x += velocity.x * dt;
    if (world->collidesWithWorld({position.x - size.x/2, position.y - size.y * 0.8, position.z - size.z / 2}, size)) {
        position.x = oldPos.x;
        velocity.x = 0;
    }

    oldPos = position;
    position.y += velocity.y * dt;
    if (world->collidesWithWorld({position.x - size.x/2, position.y - size.y * 0.8, position.z - size.z / 2}, size)) {
        position.y = oldPos.y;
        velocity.y = 0;
        if (oldPos.y >= position.y) falling = false;
    } else {
        falling = true;
    }

    oldPos = position;
    position.z += velocity.z * dt;
    if (world->collidesWithWorld({position.x - size.x/2, position.y - size.y * 0.8, position.z - size.z / 2}, size)) {
        position.z = oldPos.z;
        velocity.z = 0;
    }
}

glm::mat4 Player::getViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void Player::updateVectors()
{
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = 0.0f;
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontXY = glm::normalize(f);
    f.y = sin(glm::radians(pitch));
    front = glm::normalize(f);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}