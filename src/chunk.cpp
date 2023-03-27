#include "chunk.h"

/* bool Chunk::isHidden(const glm::uvec3 &position) const {
    assert(position[0] < CHUNK_SIZE && position[1] < CHUNK_SIZE && position[2] < CHUNK_SIZE);
    if (position[0] == Chunk::CHUNK_SIZE-1 || position[0] == 0 || position[1] == Chunk::CHUNK_SIZE-1 || position[1] == 0 || position[2] == Chunk::CHUNK_SIZE-1 || position[2] == 0) {
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

bool Chunk::isHidden(const Block &block) const {
    return this->isHidden(block.position);
}
 */
void Chunk::createMesh() {
    vertexPositions = std::vector<glm::vec3>();
    vertexColors = std::vector<glm::vec3>();

    // iterates over all blocks
    for (int x = 0; x < Chunk::CHUNK_SIZE; x++){
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
                if (this->blocks[x][y][z].type == Block::Type::AIR) {
                    continue;
                }
                glm::vec3 currentBlockPosition(x, y, z);

                if (x+1 >= Chunk::CHUNK_SIZE || this->blocks[x+1][y][z].type == Block::Type::AIR) {
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

                    textureCoordinates.push_back(glm::vec2(0, 1));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(0, 0));

                    textureCoordinates.push_back(glm::vec2(0, 0));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(1, 0));
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

                    textureCoordinates.push_back(glm::vec2(0, 1));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(0, 0));

                    textureCoordinates.push_back(glm::vec2(0, 0));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(1, 0));
                }

                if (z+1 >= Chunk::CHUNK_SIZE || this->blocks[x][y][z+1].type == Block::Type::AIR) {
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

                    textureCoordinates.push_back(glm::vec2(0, 1));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(0, 0));

                    textureCoordinates.push_back(glm::vec2(0, 0));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(1, 0));
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

                    textureCoordinates.push_back(glm::vec2(0, 1));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(0, 0));

                    textureCoordinates.push_back(glm::vec2(0, 0));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(1, 0));
                }

                if (y+1 >= Chunk::CHUNK_SIZE || this->blocks[x][y+1][z].type == Block::Type::AIR) {
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

                    textureCoordinates.push_back(glm::vec2(0, 1));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(0, 0));

                    textureCoordinates.push_back(glm::vec2(0, 0));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(1, 0));
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

                    textureCoordinates.push_back(glm::vec2(0, 1));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(0, 0));

                    textureCoordinates.push_back(glm::vec2(0, 0));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(1, 0));
                }
            }
        }
    }

}