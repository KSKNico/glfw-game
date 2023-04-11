#include "world.h"


World::World(unsigned int renderDistance, Player &player) : renderDistance(renderDistance), player(player) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 9);

    this->chunks = std::unordered_map<glm::ivec3, Chunk, IntegerVec3Hasher>();

    glm::ivec3 chunkPosition;
    for (int x = -renderDistance+1; x < (int) renderDistance; ++x) {
        for (int y = -renderDistance+1; y < (int) renderDistance; ++y) {
            for (int z = -renderDistance+1; z < (int) renderDistance; ++z) {
                chunkPosition = ((glm::ivec3) (player.position * (float) (1/Chunk::CHUNK_SIZE))) + glm::ivec3(x, y, z);

                chunks.insert(std::make_pair(chunkPosition, Chunk(chunkPosition, this->chunks)));
            }
        }
    }

    // populate all chunks
    for (auto &chunk : chunks) {
        chunk.second.populateChunk();
    }

    // create mesh and VAO for all chunks
    for (auto &chunk : chunks) {
        chunk.second.createMesh();
        chunk.second.createVAO();
    }
}