#pragma once

#include "block.h"
#include "player.h"
#include <vector>
#include <random>

const float gravitation = 1.0f;

class World {
    public:
        World(int sizeX, int sizeY, Player &player);
        std::vector<Block> blocks;
        Player &player;
        void calculatePhysics();
    private:
        
};