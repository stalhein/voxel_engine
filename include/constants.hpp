#pragma once

#include <glm/glm.hpp>

constexpr bool CULL_BACK_FACES = true;
constexpr bool WIREFRAMES = false;

constexpr int CHUNK_SIZE = 16;
constexpr int BLOCKS_PER_CHUNK = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

struct CollisionBox
{
    glm::vec3 position;
    glm::vec3 size;
};

constexpr int faces[] = {
    // Left face
    0, 0, 0, 0,
    0, 1, 0, 2,
    0, 1, 1, 3,

    0, 0, 0, 0,
    0, 1, 1, 3,
    0, 0, 1, 1,

    // Right face
    1, 0, 0, 0,
    1, 0, 1, 1,
    1, 1, 1, 3,

    1, 0, 0, 0,
    1, 1, 1, 3,
    1, 1, 0, 2,

    // Bottom face
    0, 0, 0, 0,
    0, 0, 1, 2,
    1, 0, 1, 3,

    0, 0, 0, 0,
    1, 0, 1, 3,
    1, 0, 0, 1,

    // Top face
    0, 1, 0, 0,
    1, 1, 0, 1,
    1, 1, 1, 3,

    0, 1, 0, 0,
    1, 1, 1, 3,
    0, 1, 1, 2,

    // Back face
    0, 0, 0, 0,
    1, 0, 0, 1,
    1, 1, 0, 3,

    0, 0, 0, 0,
    1, 1, 0, 3,
    0, 1, 0, 2,

    // Front face
    0, 0, 1, 0,
    0, 1, 1, 2,
    1, 1, 1, 3,

    0, 0, 1, 0,
    1, 1, 1, 3,
    1, 0, 1, 1
};