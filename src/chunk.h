#pragma once

#include "block.h"

#include <array>
#include <vector>



class Chunk {
    public:
        static constexpr int CHUNK_SIZE = 16;

        std::array<std::array<std::array<Block, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> blocks;

        std::vector<glm::vec3> vertexPositions;
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

        Chunk();
        void createMesh();

};