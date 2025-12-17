#pragma once

#include <glm/glm.hpp>

constexpr int START_SCR_WIDTH = 800, START_SCR_HEIGHT = 600;
int scr_width = START_SCR_WIDTH, scr_height = START_SCR_HEIGHT;

constexpr bool CULL_BACK_FACES = false;
constexpr bool WIREFRAMES = true;

constexpr int CHUNK_SIZE = 16;
constexpr int BLOCKS_PER_CHUNK = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

struct CollisionBox
{
    glm::vec3 position;
    glm::vec3 size;
};

constexpr int faces[] = {
    // Left 
    0, 0, 0, 0,
    0, 1, 0, 2,
    0, 1, 1, 3,
    0, 1, 1, 3,

    0, 0, 0, 0,
    0, 1, 1, 3,
    0, 0, 1, 1,

    // Right
    1, 0, 0, 0,
    1, 0, 1, 1,
    1, 1, 1, 3,

    1, 0, 0, 0,
    1, 1, 1, 3,
    1, 1, 0, 2,

    // Bottom
    0, 0, 0, 0,
    0, 0, 1, 2,
    1, 0, 1, 3,

    0, 0, 0, 0,
    1, 0, 1, 3,
    1, 0, 0, 1,

    // Top
    0, 1, 0, 0,
    1, 1, 0, 1,
    1, 1, 1, 3,

    0, 1, 0, 0,
    1, 1, 1, 3,
    0, 1, 1, 2,

    // Back
    0, 0, 0, 0,
    1, 0, 0, 1,
    1, 1, 0, 3,

    0, 0, 0, 0,
    1, 1, 0, 3,
    0, 1, 0, 3,

    // Front
    0, 0, 1, 0,
    0, 1, 1, 2,
    1, 1, 1, 3
};