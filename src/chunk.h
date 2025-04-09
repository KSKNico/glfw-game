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
#include "direction.h"
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

    using quad = std::pair<glm::u8vec3, glm::u8vec3>;
    using quadMesh = std::vector<quad>;
    using chunkSlice = std::array<std::array<Block, CHUNK_SIZE>, CHUNK_SIZE>;

    GLuint vao;
    GLuint vertexBuffer;
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

    std::vector<glm::vec<3, GLubyte, glm::packed_highp>> vertexPositions;

    std::vector<glm::vec3> vertexColors;

    // the vertex facing is the normal
    std::vector<GLubyte> vertexFacing;

    // the indices must point to one of the textures
    std::vector<GLubyte> textureIndices;

    // this contains the indices of the quads in the vertex buffer
    // the primitive mode is GL_TRIANGLES, so we need 6 indices for each quad
    std::vector<GLuint> indexBuffer;

    Chunk(const glm::ivec3 &position,
          std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IntegerVec3Hasher> &chunks,
          unsigned int worldSeed,
          std::mutex &chunkMutex);

    ~Chunk();

    Chunk(const Chunk &) = delete;
    Chunk(Chunk &&) = default;
    Chunk &operator=(const Chunk &) = delete;

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
    // the mesh should be
    void createMesh();

    void populateChunk();
};