#pragma once

#include "block.h"
#include "player.h"
#include <vector>
#include <random>
#include <map>
#include <glm/matrix.hpp>

const float gravitation = 1.0f;

class World {
    public:
        World(int sizeX, int sizeY, Player &player);
        std::map<std::tuple<int, int, int>, Block> blocks;
        Player &player;
        void calculatePhysics();
        void populateWorld(int sizeX, int sizeY, int height);

        /// @brief Calculates the closest block when filling out when drawing a line starting from position in the specified direction. 
        /// @param position The position from where to draw the line
        /// @param direction The direction in which the line is to be drawn
        /// @param maxRange Stops if no block is found whose distance is closer or equal to maxRange
        /// @return A reference to the closest block
        Block& closestBlockInLOS(glm::vec3 &position, glm::vec3 &direction, double maxRange);        
};