#include "chunk.hpp"
#include "world.hpp"

Chunk::Chunk(World* world, int x, int y, int z) : chunkX(x), chunkY(y), chunkZ(z), world(world)
{
    model = glm::translate(glm::mat4(1.0f), glm::vec3(chunkX * CHUNK_SIZE, chunkY * CHUNK_SIZE, chunkZ * CHUNK_SIZE));

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(uint32_t), (void*)0);
    glEnableVertexAttribArray(0);
}

Chunk::~Chunk()
{

}

void Chunk::generateTerrain()
{
    blocks.fill(0);
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            float noiseValue = world->noise.GetNoise((float)(x + chunkX * CHUNK_SIZE), (float)(z + chunkZ * CHUNK_SIZE));
            int height = std::max(floor((noiseValue + 1.0) / 2 * (CHUNK_SIZE * 8)), 1.0);
            int localHeight = std::clamp(height - chunkY * CHUNK_SIZE, 0, CHUNK_SIZE);
            for (int y = 0; y < localHeight; ++y) {
                blocks[posToIndex(x, y, z)] = 1;
            }
        }
    }
}

void Chunk::generateMesh()
{
    mesh.clear();
    for (int z = 0; z < CHUNK_SIZE; ++z) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int x = 0; x < CHUNK_SIZE; ++x) {
                int index = posToIndex(x, y, z);
                if (blocks[index] == 0) continue;
                uint8_t blockType = blocks[index];
                if (getBlockAt(x-1, y, z) == 0) addFace(x, y, z, 0, blockType - 1);
                if (getBlockAt(x+1, y, z) == 0) addFace(x, y, z, 1, blockType - 1);
                if (getBlockAt(x, y-1, z) == 0) addFace(x, y, z, 2, blockType - 1);
                if (getBlockAt(x, y+1, z) == 0) addFace(x, y, z, 3, blockType - 1);
                if (getBlockAt(x, y, z-1) == 0) addFace(x, y, z, 4, blockType - 1);
                if (getBlockAt(x, y, z+1) == 0) addFace(x, y, z, 5, blockType - 1);
                
            }
        }
    }
}

void Chunk::uploadMesh()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(uint32_t), mesh.data(), GL_DYNAMIC_DRAW);
}

void Chunk::render(Shader* shader)
{
    shader->setMat4("uModel", model);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.size());
}

void Chunk::addFace(int x, int y, int z, int normalIndex, int textureIndex)
{
    int base = normalIndex * 24;
    for (int i = base; i < base+24; i+=4) {
        int vx = x + faces[i];
        int vy = y + faces[i+1];
        int vz = z + faces[i+2];
        int uv = faces[i+3];
        mesh.push_back(packVertex(vx, vy, vz, normalIndex, uv, textureIndex));
    }
}

std::array<uint8_t, BLOCKS_PER_CHUNK>& Chunk::getBlocks()
{
    return blocks;
}

uint8_t Chunk::getLocalBlockAt(int x, int y, int z)
{
    if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE) {
        return 0;
    }
    int index = posToIndex(x, y, z);
    return blocks[index];
}

uint8_t Chunk::getBlockAt(int x, int y, int z)
{
    int cx = chunkX, cy = chunkY, cz = chunkZ;
    int lx = x, ly = y, lz = z;

    if (x < 0)  cx -= 1, lx += CHUNK_SIZE;
    else if (x >= CHUNK_SIZE)   cx += 1, lx -= CHUNK_SIZE;
    else if (y < 0) cy -= 1, ly += CHUNK_SIZE;
    else if (y >= CHUNK_SIZE)   cy += 1, ly -= CHUNK_SIZE;
    else if (z < 0) cz -= 1, lz += CHUNK_SIZE;
    else if (z >= CHUNK_SIZE)   cz += 1, lz -= CHUNK_SIZE;

    if (cx != chunkX || cy != chunkY || cz != chunkZ) {
        Chunk* chunk = world->getChunkAt(cx, cy, cz);
        if (!chunk) return 0;
        return chunk->getLocalBlockAt(lx, ly, lz);
    }

    int index = posToIndex(x, y, z);
    return blocks[index];
}

void Chunk::setBlockAt(int x, int y, int z, uint8_t value)
{
    if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE)   return;
    int index = posToIndex(x, y, z);
    blocks[index] = value;
}