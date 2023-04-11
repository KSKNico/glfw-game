#pragma once

#include "block.h"
#include "util.h"

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <random>
#include <unordered_map>



class Chunk {
    public:
        static constexpr const unsigned char CHUNK_SIZE = 16;

        GLuint vao;
        GLuint vertexBuffer;
        GLuint textureCoordinatesBuffer;

        glm::ivec3 chunkPosition;

        std::unordered_map<glm::ivec3, Chunk, IntegerVec3Hasher>& chunks;

        std::array<std::array<std::array<Block, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> blocks;

        std::vector<glm::vec<3, GLubyte, glm::packed_highp>> vertexPositions;
        std::vector<glm::vec3> vertexColors;
        std::vector<glm::vec<2, GLubyte, glm::packed_highp>> textureCoordinates;

        /// @brief For the position of a block returns true when that block is only surrounded by blocks that are solid
        /// @param position Position of the that block
        /// @return True if block is hidden, false otherwise
        // bool isHidden(const glm::uvec3 &position) const;

        /// @brief For a block returns true when that block is only surrounded by blocks that are solid
        /// @param block Block to be checked 
        /// @return True if block is hidden, false otherwise
        // bool isHidden(const Block &block) const;

        Chunk(const glm::ivec3 &chunkPosition, std::unordered_map<glm::ivec3, Chunk, IntegerVec3Hasher> &chunks);
        void createMesh();

        void populateChunk();
        void createVAO();

};