#pragma once

#include <array>
#include <chrono>
#include <memory>
#include <mutex>
#include <random>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "block.h"
#include "glad.h"
#include "integer_vec3_hasher.h"
#include "perlin.h"
#include "util.h"

class Chunk {
   public:
    static constexpr unsigned char CHUNK_SIZE = 16;
    static constexpr std::array<glm::ivec3, 6> ADJACENT_CHUNK_POSITIONS = {
        glm::ivec3(0, 0, 1), glm::ivec3(0, 0, -1), glm::ivec3(0, 1, 0),
        glm::ivec3(0, -1, 0), glm::ivec3(1, 0, 0), glm::ivec3(-1, 0, 0)};

    using quad = std::pair<glm::u8vec2, glm::u8vec2>;
    using quadMesh = std::vector<quad>;
    using chunkSlice = std::array<std::array<Block, CHUNK_SIZE>, CHUNK_SIZE>;

    enum class Direction {
        POS_X,
        NEG_X,
        POS_Y,
        NEG_Y,
        POS_Z,
        NEG_Z
    };

    static constexpr std::array<glm::ivec3, 6> DIRECTION_VECTORS = {
        glm::ivec3(1, 0, 0),
        glm::ivec3(-1, 0, 0),
        glm::ivec3(0, 1, 0),
        glm::ivec3(0, -1, 0),
        glm::ivec3(0, 0, 1),
        glm::ivec3(0, 0, -1)};

    GLuint vao;
    GLuint vertexBuffer;
    GLuint textureCoordinatesBuffer;
    GLuint vertexFacingBuffer;
    GLuint textureIndexBuffer;

    unsigned int vertexCount;

    // position is in chunk coordiantes, world coordinates are position * CHUNK_SIZE
    glm::ivec3 position;
    std::array<glm::ivec3, 8>
        chunkVertices;

    std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IntegerVec3Hasher> &chunks;

    unsigned int worldSeed;
    std::array<std::array<std::array<Block, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE>
        blocks;
    std::mutex &chunkMutex;

    std::vector<glm::vec<3, GLubyte, glm::packed_highp>>
        vertexPositions;
    std::vector<glm::vec3>
        vertexColors;
    std::vector<glm::vec<2, GLubyte, glm::packed_highp>>
        textureCoordinates;
    std::vector<GLubyte>
        vertexFacing;
    std::vector<GLubyte>
        textureIndices;

    Chunk(const glm::ivec3 &position,
          std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IntegerVec3Hasher> &chunks,
          unsigned int worldSeed,
          std::mutex &chunkMutex);

    ~Chunk();

    Chunk(const Chunk &) = delete;
    Chunk(Chunk &&) = default;
    Chunk &operator=(const Chunk &) = delete;
    GLint createVAO();
    void deleteVAO();

    bool isVisible(const Block &block, Direction direction);

    // returns true if the position is within the bounds of the chunk, the position must be in chunk coordinates
    static bool isInside(const glm::ivec3 &blockPositionInChunk);

    // returns the world coordinates of a block in the chunk
    // the block position must be in the range of [0, CHUNK_SIZE-1] for all dimensions
    glm::ivec3 convertChunkToWorldCoordinates(const glm::ivec3 &blockPositionInChunk);

    // this returns a quad mesh with the correct quads for one dimension
    quadMesh greedyMeshing(unsigned int sliceIndex, Direction direction);

    // returns a block that is in slice s, at position i, k
    // direction says which axis is cut
    Block getBlockBySlice(int s, int i, int k, Direction direction);

    // gets a slice of blocks that is perpendicular to the given direction
    Chunk::chunkSlice getSlice(unsigned int toSlice, Direction direction);

   private:
    void createMesh();

    void populateChunk();
};