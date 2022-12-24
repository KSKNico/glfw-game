#pragma once

#include "block.h"
#include "player.h"
#include <vector>
#include <random>
#include <map>

const float gravitation = 1.0f;

class World {
    public:
        World(int sizeX, int sizeY, Player &player);
        std::map<std::tuple<int, int, int>, Block> blocks;
        Player &player;
        void calculatePhysics();
        void populateWorld(int sizeX, int sizeY, int height);
    private:
        
};