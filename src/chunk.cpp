#include "chunk.hpp"

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
    blocks.fill(1);


}

void Chunk::generateMesh()
{
    mesh.clear();
    for (int z = 0; z < CHUNK_SIZE; ++z) {
        for (int y = 0; y < CHUNK_SIZE; ++z) {
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
    shader->setMat4("uMode", model);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.size());
}

void Chunk::addFace(int x, int y, int z, int normalIndex, int textureIndex)
{
    int base = normalIndex * 24;
    for (int i = base; i < base+24; i+=4) {
        int vx = x + faces[i];
        int vy = y + faces[i+1];
        
    }
}