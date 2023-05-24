#pragma once

#include "block.h"
#include "player.h"
#include "chunk.h"
#include "util.h"
#include "skybox.h"

#include <vector>
#include <memory>
#include <random>
#include <unordered_map>
#include <iostream>
#include <glm/matrix.hpp>
#include <chrono>

const float GRAVITATION = 1.0f;

class World {
    public:
        World(unsigned int renderDistance, Player &player, Camera &camera);
        // std::vector<std::vector<std::vector<Block>>> blocks;

        unsigned int renderDistance;
        unsigned int seed;

        Player &player;
        Camera &camera;
        Skybox skybox;
        std::vector<std::shared_ptr<Block>> visibleBlocks;
        /* std::vector<glm::vec3> vertexPositions;
        std::vector<glm::vec3> vertexColors;
        std::vector<glm::vec<2, GLubyte, glm::packed_highp>> textureCoordinates;
        */
        std::unordered_map<glm::ivec3, std::unique_ptr<Chunk> , IntegerVec3Hasher> chunks;

        // void createMesh();

        /// @brief Calculates the closest block when filling out when drawing a line starting from position in the specified direction
        /// @param position Position from where to draw the line
        /// @param direction Direction in which the line is to be drawn
        /// @param maxRange Stops if no block is found whose distance is closer or equal to maxRange
        /// @return A reference to the closest block
        // Block& closestBlockInLOS(glm::vec3 &position, glm::vec3 &direction, double maxRange);        

        /// @brief Loads correct chunks in render distance range
        void loadChunks();

        /// @brief Unloads chunks too far away from the player
        void unloadChunks();

        /// @brief Updates the world by loading and unloading chunks
        void update();

        static glm::ivec3 convertToChunkCoordinates(const glm::vec3 &position);

        void chunkLoader();

};