#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <vector>

#include <glm/glm.hpp>

#include "constants.hpp"
#include "fastnoiselite.hpp"
#include "shader.hpp"
#include "input.hpp"
#include "chunk.hpp"
#include "stb_image.hpp"

struct ChunkCoord
{
    int x, y, z;

    bool operator==(const ChunkCoord& o) const noexcept
    {
        return x == o.x && y == o.y && z == o.z;
    }
};

struct ChunkCoordHash
{
    size_t operator()(const ChunkCoord& c) const noexcept {
        uint64_t h =0;
        h ^= std::hash<int>()(c.x) + 0x9e3779b97f4a7c15ULL + (h<<6) + (h>>2);
        h ^= std::hash<int>()(c.y) + 0x9e3779b97f4a7c15ULL + (h<<6) + (h>>2);
        h ^= std::hash<int>()(c.z) + 0x9e3779b97f4a7c15ULL + (h<<6) + (h>>2);
        return h;
    }
};

class World
{
public:
    FastNoiseLite noise;

    World();
    ~World();

    void update();
    void render(Shader* shader);

    Chunk* getChunkAt(int cx, int cy, int cz);
    uint8_t getBlockAt(int x, int y, int z);

    bool collidesWithWorld(const glm::vec3& position, glm::vec3& size);
    
private:
    std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> chunks;

    GLuint texArray;

    std::vector<std::vector<std::vector<uint8_t>>> getBlocksAround(CollisionBox& cuboid);
    inline bool inChunkBounds(int x, int y, int z) noexcept { return (unsigned)x < CHUNK_SIZE && (unsigned)y < CHUNK_SIZE && (unsigned)z < CHUNK_SIZE; }

    void loadTextures();
};