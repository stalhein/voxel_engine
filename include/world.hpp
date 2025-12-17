#pragma once

#include <glm/glm.hpp>

#include "constants.hpp"

class World
{
public:
    World();
    bool collidesWithWorld(const glm::vec3& position, glm::vec3& size);
private:
};