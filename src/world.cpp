#include "world.h"


World::World(int sizeX, int sizeY, Player &player) : player(player) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 10);

    for (int i = 0; i <= sizeX; i++){
        for (int j = 0; j <= sizeY; j++) {
            this->blocks.push_back(
                Block(
                    glm::vec3(i, distr(gen), j)
                )
            );
        }
    }
}

void World::calculatePhysics() {
    // If next location is in bounding box of cube, set player's speed to 0
    glm::vec3 nextLocation = player.position + player.speed;
    for (auto block : blocks) {
        if (nextLocation[0] < block.position[0]+1 && 
        nextLocation[0] > block.position[0] &&
        nextLocation[1] < block.position[1]+1 && 
        nextLocation[1] > block.position[1] &&
        nextLocation[2] < block.position[2]+1 && 
        nextLocation[2] > block.position[2]) {
            player.speed[0] = 0.0f, player.speed[1] = 0.0f, player.speed[2] = 0.0f;
        }
    }
    player.movePlayer(nextLocation);
    // this is the gravity added onto the speed
    player.speed += glm::vec3(0.0f, 0.01f, 0.0f);
}


