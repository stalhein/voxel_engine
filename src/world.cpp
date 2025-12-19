#include "world.hpp"

const int textureCount = 9;

const char* texturePaths[textureCount] = {
    "assets/grass_1.png",
    "assets/grass_2.png",
    "assets/stone_bricks_2.png",
    "assets/stone_bricks_3.png",
    "assets/stone_bricks_4.png",
    "assets/sand_1.png",
    "assets/sand_2.png",
    "assets/wood_1.png",
    "assets/wood_2.png"
};

World::World()
{
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.01f);

    loadTextures();

    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < 1; ++y) {
            for (int z = 0; z < 1; ++z) {
                chunks[{x, y, z}] = std::make_unique<Chunk>(this, x, y, z);
                chunks[{x, y, z}]->generateTerrain();
            }
        }
    }
}

World::~World()
{   
}

void World::update()
{
    for (auto& [coord, chunk] : chunks) {
        if (chunk->dirty) {
            chunk->generateMesh();
            chunk->uploadMesh();
            chunk->dirty = false;
        }
    }
}

void World::render(Shader* shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texArray);
    shader->setInt("TextureArray", 0);

    for (auto& [coord, chunk] : chunks) {
        chunk->render(shader);
    }
}

void World::loadTextures()
{
    glGenTextures(1, &texArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texArray);

    int width, height, channels;

    unsigned char* firstData = stbi_load(texturePaths[0], &width, &height, &channels, 4);
    if (!firstData) {
        std::cerr << "Failed to load texture: " << texturePaths[0] << std::endl;
        exit(1);
    }

    int mipLevels = 1 + (int)std::floor(std::log2(std::max(width, height)));

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevels, GL_RGBA8, width, height, textureCount);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, firstData);

    stbi_image_free(firstData);

    for (int i = 1; i < textureCount; ++i) {
        unsigned char* data = stbi_load(texturePaths[i], &width, &height, &channels, 4);
        if (!data) {
            std::cerr << "Failed to load texture: " << texturePaths[i] << std::endl;
            exit(1);
        }
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

Chunk* World::getChunkAt(int cx, int cy, int cz) {
    ChunkCoord key{cx, cy, cz};

    auto it = chunks.find(key);
    if (it != chunks.end()) return it->second.get();

    return nullptr;
}

uint8_t World::getBlockAt(int x, int y, int z)
{
    int chunkX = std::floor(x/CHUNK_SIZE);
    int chunkY = std::floor(y/CHUNK_SIZE);
    int chunkZ = std::floor(z/CHUNK_SIZE);

    Chunk* chunk = getChunkAt(chunkX, chunkY, chunkZ);
    if (!chunk) return 0;

    int localX = x-chunkX*CHUNK_SIZE;
    int localY = y-chunkY*CHUNK_SIZE;
    int localZ = z-chunkZ*CHUNK_SIZE;

    return chunk->getLocalBlockAt(localX, localY, localZ);
}

bool World::collidesWithWorld(const glm::vec3& pos, glm::vec3& size)
{
    CollisionBox box = {pos, size};
    auto blocks = getBlocksAround(box);

    for (size_t x = 0; x < blocks.size(); ++x)
    {
        for (size_t y = 0; y < blocks[x].size(); ++y)
        {
            for (size_t z = 0; z < blocks[x][y].size(); ++z)
            {
                if (blocks[x][y][z] != 0)   return true;
            }
        }
    }

    return false;
}

std::vector<std::vector<std::vector<uint8_t>>> World::getBlocksAround(CollisionBox& cuboid)
{
    int startX = static_cast<int>(std::floor(cuboid.position.x));
    int startY = static_cast<int>(std::floor(cuboid.position.y));
    int startZ = static_cast<int>(std::floor(cuboid.position.z));
    
    int endX = static_cast<int>(std::floor(cuboid.position.x + cuboid.size.x));
    int endY = static_cast<int>(std::floor(cuboid.position.y + cuboid.size.y));
    int endZ = static_cast<int>(std::floor(cuboid.position.z + cuboid.size.z));
    
    int sx = endX - startX + 1;
    int sy = endY - startY + 1;
    int sz = endZ - startZ + 1;

    std::vector<std::vector<std::vector<uint8_t>>> overlappingBlocks(
        sx,
        std::vector<std::vector<uint8_t>>(sy, std::vector<uint8_t>(sz))
    );

    for (int x = 0; x < sx; ++x) {
        for (int y = 0; y < sy; ++y) {
            for (int z = 0; z < sz; ++z) {
                int wx = startX + x;
                int wy = startY + y;
                int wz = startZ + z;

                overlappingBlocks[x][y][z] = getBlockAt(wx, wy, wz);
            }
        }
    }

    return overlappingBlocks;
}
