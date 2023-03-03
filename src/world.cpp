#include "world.h"


World::World(int sizeX, int sizeY, int sizeZ, Player &player) : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ), player(player) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 9);

    this->blocks = std::vector<std::vector<std::vector<Block>>>();

    for (int i = 0; i < sizeX; i++){
        std::vector<std::vector<Block>> xVector = std::vector<std::vector<Block>>();
        for (int j = 0; j < sizeY; j++) {
            std::vector<Block> yVector = std::vector<Block>();
            for (int k = 0; k < sizeZ; k++) {
                int randomNumber = distr(gen);
                if (randomNumber >= 10) {
                    yVector.push_back(Block(glm::vec3(i, j, k), Block::Type::AIR));
                } else {
                    yVector.push_back(Block(glm::vec3(i, j, k), Block::Type::SOLID));
                }
            }
            xVector.push_back(yVector);
        }
        this->blocks.push_back(xVector);
    }

    // steps over all blocks and determines visibility
    for (int i = 0; i < sizeX; i++){
        for (int j = 0; j < sizeY; j++) {
            for (int k = 0; k < sizeZ; k++) {
                this->blocks[i][j][k].hidden = isHidden(glm::vec3(i, j, k));
            }
        }
    }

}



bool World::isHidden(const glm::vec3 &position) const {
    if (position[0] == sizeX-1 || position[0] == 0 || position[1] == sizeY-1 || position[1] == 0 || position[2] == sizeZ-1 || position[2] == 0) {
        return false;
    } else if (
        blocks[position[0]+1][position[1]][position[2]].isSolid()
        &&
        blocks[position[0]-1][position[1]][position[2]].isSolid()
        &&
        blocks[position[0]][position[1]+1][position[2]].isSolid()
        &&
        blocks[position[0]][position[1]-1][position[2]].isSolid()
        &&
        blocks[position[0]][position[1]][position[2]+1].isSolid()
        &&
        blocks[position[0]][position[1]][position[2]-1].isSolid()
    ) {
        return true;
    } else {
        return false;
    }

}

bool World::isHidden(const Block &block) const {
    return this->isHidden(block.position);
}

// void World::populateWorld(int sizeX, int sizeY, int sizeZ, int height) {
    // this->blocks = std::vector<Block>();
    // std::random_device rd;
    // std::mt19937 gen(rd()); // seed the generator
    // std::uniform_int_distribution<> distr(0, height);
    // for (int i = 0; i <= sizeX; i++) {
    //     for (int j = 0; j <= sizeY; j++) {
    //         for (int k = 0; k <= distr(gen); k++) {
    //             this->blocks.push_back(
    //                 Block(
    //                     glm::vec3(i, k, j)
    //                 )
    //             );
    //         }
    //     }
    // }
// }

void World::calculatePhysics() {
    // If next location is in bounding box of cube, set player's speed to 0
/*     glm::vec3 nextLocation = player.position + player.speed;
    for (auto pair : blocks) {
        if (nextLocation[0] < pair.second.position[0]+1 && 
        nextLocation[0] > pair.second.position[0] &&
        nextLocation[1] < pair.second.position[1]+1 && 
        nextLocation[1] > pair.second.position[1] &&
        nextLocation[2] < pair.second.position[2]+1 && 
        nextLocation[2] > pair.second.position[2]) {
            player.speed[0] = 0.0f, player.speed[1] = 0.0f, player.speed[2] = 0.0f;
        }
    }
    player.movePlayer(nextLocation);
    // this is the gravity added onto the speed
    player.speed += glm::vec3(0.0f, -0.01f, 0.0f); */
}



Block& closestBlockInLOS(glm::vec3 &position, glm::vec3 &direction, double maxRange) {
    // bound check
/*     double x1 = int(position[0]-maxRange);
    double x2 = int(position[0]+maxRange);

    double y1 = int(position[1]-maxRange);
    double y2 = int(position[1]+maxRange);

    double z1 = int(position[2]-maxRange);
    double z2 = int(position[2]+maxRange);

    for (int x = x1; x <= x2; x++) {
        for (int y = y1; y <= y2; y++) {
            for (int z = z1; z <= z2; z++) {
                
            }
        }
    } */
}
