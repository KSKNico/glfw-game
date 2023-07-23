#pragma once

#include <array>
#include <chrono>
#include <memory>
#include <mutex>
#include <random>
#include <unordered_map>
#include <vector>

#include "block.h"
#include "glad.h"
#include "integer_vec3_hasher.h"
#include "perlin.h"
#include "util.h"
#include "vertex.h"

class Chunk {
   public:
    static constexpr unsigned char CHUNK_SIZE = 16;
    static constexpr std::array<glm::ivec3, 6> ADJACENT_CHUNK_POSITIONS = {
        glm::ivec3(0, 0, 1), glm::ivec3(0, 0, -1), glm::ivec3(0, 1, 0),
        glm::ivec3(0, -1, 0), glm::ivec3(1, 0, 0), glm::ivec3(-1, 0, 0)};

    GLuint vao;
    GLuint vertexBuffer;
    GLuint textureCoordinatesBuffer;
    GLuint vertexFacingBuffer;
    GLuint textureIndexBuffer;

    unsigned int vertexCount;

    // position is in chunk coordiantes, world coordinates are position * CHUNK_SIZE
    glm::ivec3 position;
    std::array<glm::ivec3, 8> chunkVertices;

    std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IntegerVec3Hasher> &chunks;

    unsigned int worldSeed;
    std::array<std::array<std::array<Block, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> blocks;
    std::mutex &chunkMutex;

    std::vector<Vertex> vertices;

    Chunk(const glm::ivec3 &position,
          std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IntegerVec3Hasher> &chunks,
          unsigned int worldSeed, std::mutex &chunkMutex);

    ~Chunk();

    Chunk(const Chunk &) = delete;
    Chunk(Chunk &&) = default;
    Chunk &operator=(const Chunk &) = delete;
    bool isSurrounded();
    void optimizeMesh();
    static bool isVertexCongruent(const glm::vec3 &v1, const glm::vec3 &v2);

   private:
    void createMesh();

    void populateChunk();
};