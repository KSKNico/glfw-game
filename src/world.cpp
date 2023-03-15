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

    // creates a vector with pointers to all visible blocks
    visibleBlocks = std::vector<std::shared_ptr<Block>>();
    for (int i = 0; i < sizeX; i++){
        for (int j = 0; j < sizeY; j++) {
            for (int k = 0; k < sizeZ; k++) {
                if (!this->blocks[i][j][k].hidden) {
                    visibleBlocks.push_back(std::shared_ptr<Block>(&blocks[i][j][k]));
                }
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

void World::createMesh() {
    vertexPositions = std::vector<glm::vec3>();
    vertexColors = std::vector<glm::vec3>();

    // iterates over all blocks
    for (int x = 0; x < sizeX; x++){
        for (int y = 0; y < sizeY; y++) {
            for (int z = 0; z < sizeZ; z++) {
                glm::vec3 &currentBlockPosition = this->blocks[x][y][z].position;

                if (x+1 >= sizeX || this->blocks[x+1][y][z].type == Block::Type::AIR) {
                    // right face
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,0,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,0,0));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,1,1));
                    
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,1,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,0,0));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,1,0));

                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                }

                if (x-1 < 0 || this->blocks[x-1][y][z].type == Block::Type::AIR) {
                    // left face
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,0,0));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,0,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,1,0));

                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,1,0));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,0,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,1,1));

                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                }

                if (z+1 >= sizeZ || this->blocks[x][y][z+1].type == Block::Type::AIR) {
                    // front face
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,0,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,0,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,1,1));
                    
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,1,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,0,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,1,1));

                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                }

                if (z-1 < 0 || this->blocks[x][y][z-1].type == Block::Type::AIR) {
                    // back face
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,0,0));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,0,0));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,1,0));
                    
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,1,0));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,0,0));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,1,0));

                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                }

                if (y+1 >= sizeY || this->blocks[x][y+1][z].type == Block::Type::AIR) {
                    // top face
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,1,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,1,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,1,0));
                    
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,1,0));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,1,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,1,0));

                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                }

                if (y-1 < 0 || this->blocks[x][y-1][z].type == Block::Type::AIR) {
                    // bottom face
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,0,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,0,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,0,0));

                    vertexPositions.push_back(currentBlockPosition+glm::vec3(1,0,0));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,0,1));
                    vertexPositions.push_back(currentBlockPosition+glm::vec3(0,0,0));

                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                }
            }
        }
    }
}
