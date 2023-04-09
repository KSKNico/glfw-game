#pragma once

#include "block.h"
#include "player.h"
#include "chunk.h"
#include "util.h"

#include <vector>
#include <memory>
#include <random>
#include <unordered_map>
#include <iostream>
#include <glm/matrix.hpp>

const float GRAVITATION = 1.0f;

class World {
    public:
        World(unsigned int renderDistance, Player &player);
        // std::vector<std::vector<std::vector<Block>>> blocks;

        unsigned int renderDistance;

        Player &player;
        std::vector<std::shared_ptr<Block>> visibleBlocks;
        /* std::vector<glm::vec3> vertexPositions;
        std::vector<glm::vec3> vertexColors;
        std::vector<glm::vec<2, GLubyte, glm::packed_highp>> textureCoordinates;
        */
        std::unordered_map<glm::ivec3, Chunk, IntegerVec3Hasher> chunks;

        // void createMesh();

        /// @brief Calculates the closest block when filling out when drawing a line starting from position in the specified direction
        /// @param position Position from where to draw the line
        /// @param direction Direction in which the line is to be drawn
        /// @param maxRange Stops if no block is found whose distance is closer or equal to maxRange
        /// @return A reference to the closest block
        // Block& closestBlockInLOS(glm::vec3 &position, glm::vec3 &direction, double maxRange);        
};