#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <vector>
#include <bits/stdc++.h>

#include "shader.hpp"
#include "constants.hpp"

class World;

class Chunk
{
public:
    int chunkX, chunkY, chunkZ;

    bool dirty = true;

    Chunk(World* world, int x, int y, int z);
    ~Chunk();
    void generateTerrain();
    void generateMesh();
    void uploadMesh();
    void render(Shader* shader);

    uint8_t getLocalBlockAt(int x, int y, int z);
    void setBlockAt(int x, int y, int z, uint8_t value);
    
    std::array<uint8_t, BLOCKS_PER_CHUNK>& getBlocks();
private:
    glm::mat4 model;

    std::array<uint8_t, BLOCKS_PER_CHUNK> blocks;
    std::vector<uint32_t> mesh;

    GLuint vbo, vao;

    World* world;

    uint8_t getBlockAt(int x, int y, int z);
    void addFace(int x, int y, int z, int normalIndex, int textureIndex);
    inline uint32_t packVertex(int localX, int localY, int localZ, int normalIndex, int uvIndex, int textureIndex)
    {
        return (uint32_t)((localX & 31) | ((localY & 31) << 5) | ((localZ & 32) << 10) | ((normalIndex & 7) << 15) | ((uvIndex << 3) << 18) | ((textureIndex & 127) << 20));
    }
    inline int posToIndex(int x, int y, int z)  { return z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x; }
};