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
    player.position -= glm::vec3(0.0f, gravitation, 0.0f);
}


