#pragma once

#include "block.h"
#include "util.h"
#include "perlin.h"

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <random>
#include <unordered_map>
#include <memory>



class Chunk {
    public:
        static constexpr const unsigned char CHUNK_SIZE = 16;

        GLuint vao;
        GLuint vertexBuffer;
        GLuint textureCoordinatesBuffer;
        GLuint vertexFacingBuffer;

        unsigned int vertexCount;

        glm::ivec3 chunkPosition;

        std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IntegerVec3Hasher>& chunks;

        std::array<std::array<std::array<Block, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> blocks;

        std::vector<glm::vec<3, GLubyte, glm::packed_highp>> vertexPositions;
        std::vector<glm::vec3> vertexColors;
        std::vector<glm::vec<2, GLubyte, glm::packed_highp>> textureCoordinates;
        std::vector<GLubyte> vertexFacing;

        Chunk(const glm::ivec3 &chunkPosition, std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IntegerVec3Hasher> &chunks);
        ~Chunk();

        Chunk(const Chunk&) = delete;
        Chunk(Chunk&&) = default;
        Chunk& operator=(const Chunk&) = delete;

        void createMesh();

        void populateChunk();
        void createVAO();

};