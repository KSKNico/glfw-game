#include "world.h"


World::World(size_x, size_y) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 10);

    for (int i = 0; i <= size_x; i++){
        for (int j = 0; j <= size_y; j++) {
            Block::blocks.push(
                Block{
                    glm::mat3(i, distr(gen), j);
                }
            );
        }
    }
}


