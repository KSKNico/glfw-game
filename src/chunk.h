#pragma once

#include "block.h"
#include "glad.h"
#include "perlin.h"
#include "util.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <array>
#include <chrono>
#include <memory>
#include <mutex>
#include <random>
#include <unordered_map>
#include <vector>

class Chunk {
   public:
    static constexpr const unsigned char CHUNK_SIZE = 16;

    GLuint vao;
    GLuint vertexBuffer;
    GLuint textureCoordinatesBuffer;
    GLuint vertexFacingBuffer;
    GLuint textureIndexBuffer;
    bool hasVAO;
    bool hasMesh;

    unsigned int vertexCount;

    // position is in chunk coordiantes, world coordinates are position * CHUNK_SIZE
    glm::ivec3 position;
    std::array<glm::ivec3, 8> chunkVertices;

    std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IntegerVec3Hasher> &chunks;

    unsigned int worldSeed;
    std::array<std::array<std::array<Block, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> blocks;
    std::mutex &chunkMutex;

    std::vector<glm::vec<3, GLubyte, glm::packed_highp>> vertexPositions;
    std::vector<glm::vec3> vertexColors;
    std::vector<glm::vec<2, GLubyte, glm::packed_highp>> textureCoordinates;
    std::vector<GLubyte> vertexFacing;
    std::vector<GLubyte> textureIndices;

    Chunk(const glm::ivec3 &position,
          std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IntegerVec3Hasher> &chunks,
          unsigned int worldSeed, std::mutex &chunkMutex);

    ~Chunk();

    Chunk(const Chunk &) = delete;
    Chunk(Chunk &&) = default;
    Chunk &operator=(const Chunk &) = delete;

    void createMesh();

    void populateChunk();
    void createVAO();
};