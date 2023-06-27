#include "world.h"

World::World(unsigned int renderDistance, Player &player, Camera &camera) : renderDistance(renderDistance), player(player), camera(camera) {
    seed = std::chrono::seconds(std::time(NULL)).count();

    this->chunks = std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IntegerVec3Hasher>();
    this->skybox = Skybox();

    // this->loadChunks();
}

void World::unloadChunks() {
    auto itr = chunks.begin();
    glm::ivec3 currentChunk = convertToChunkCoordinates(camera.position);
    while (itr != chunks.end()) {
        if (itr->first.x < currentChunk.x - (int)renderDistance || itr->first.x > currentChunk.x + (int)renderDistance ||
            itr->first.y < currentChunk.y - (int)renderDistance || itr->first.y > currentChunk.y + (int)renderDistance ||
            itr->first.z < currentChunk.z - (int)renderDistance || itr->first.z > currentChunk.z + (int)renderDistance) {
            itr = chunks.erase(itr);
        } else {
            ++itr;
        }
    }
}

// TODO: gradually load chunks to avoid lag spikes
void World::loadChunks() {
    glm::ivec3 position;
    for (int x = -renderDistance; x <= (int)renderDistance; ++x) {
        for (int y = -renderDistance; y <= (int)renderDistance; ++y) {
            for (int z = -renderDistance; z <= (int)renderDistance; ++z) {
                glm::ivec3 clampedCoordinates = World::convertToChunkCoordinates(camera.position);
                position = clampedCoordinates + glm::ivec3(x, y, z);

                if (chunks.find(position) == chunks.end()) {
                    // std::unique_ptr<Chunk> chunk = std::make_unique<Chunk> (Chunk(position, this->chunks));
                    std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(position, this->chunks, seed, chunkMutex);
                    chunks.emplace(std::make_pair(position, std::move(chunk)));
                }
            }
        }
    }
}

void World::update() {
    this->unloadChunks();
    this->loadChunks();
}

glm::ivec3 World::convertToChunkCoordinates(const glm::vec3 &coordinates) {
    return glm::ivec3(std::floor(coordinates.x / Chunk::CHUNK_SIZE), std::floor(coordinates.y / Chunk::CHUNK_SIZE), std::floor(coordinates.z / Chunk::CHUNK_SIZE));
}

void World::chunkLoader() {
    while (true) {
        this->update();
    }
}