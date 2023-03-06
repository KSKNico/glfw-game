#pragma once

#include "block.h"
#include "player.h"
#include <vector>
#include <memory>
#include <random>
#include <map>
#include <glm/matrix.hpp>

const float gravitation = 1.0f;

class World {
    public:
        World(int sizeX, int sizeY, int sizeZ, Player &player);
        std::vector<std::vector<std::vector<Block>>> blocks;
        int sizeX;
        int sizeY;
        int sizeZ;
        Player &player;
        std::vector<std::shared_ptr<Block>> visibleBlocks;

        void calculatePhysics();
        // void populateWorld(int sizeX, int sizeY, int sizeZ, int height);

        /// @brief For the position of a block returns true when that block is only surrounded by blocks that are solid
        /// @param position Position of the that block
        /// @return True if block is hidden, false otherwise
        bool isHidden(const glm::vec3 &position) const;

        /// @brief For a block returns true when that block is only surrounded by blocks that are solid
        /// @param block Block to be checked 
        /// @return True if block is hidden, false otherwise
        bool isHidden(const Block &block) const;

        /// @brief Calculates the closest block when filling out when drawing a line starting from position in the specified direction
        /// @param position Position from where to draw the line
        /// @param direction Direction in which the line is to be drawn
        /// @param maxRange Stops if no block is found whose distance is closer or equal to maxRange
        /// @return A reference to the closest block
        // Block& closestBlockInLOS(glm::vec3 &position, glm::vec3 &direction, double maxRange);        
};