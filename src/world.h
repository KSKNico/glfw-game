#pragma once

#include "block.h"
#include <vector>
#include <random>

class World {
    public:
        World(int sizeX, int sizeY);
        std::vector<Block> blocks;
        

};