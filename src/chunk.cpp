#include "chunk.h"
#include <iostream>

Chunk::Chunk(const glm::ivec3 &chunkPosition, std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IntegerVec3Hasher> &chunks) : chunkPosition(chunkPosition), chunks(chunks) {
    std::cout << "Created chunk at position " << chunkPosition[0] << " " << chunkPosition[1] << " " << chunkPosition[2] << std::endl;
    this->populateChunk();
    this->createMesh();
    this->createVAO();
}

Chunk::~Chunk() {
    // free resources used on the GPU
    std::cout << "Deleted chunk at position " << chunkPosition[0] << " " << chunkPosition[1] << " " << chunkPosition[2] << std::endl;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &textureCoordinatesBuffer);
}

void Chunk::populateChunk() {
    static std::random_device rd; // obtain a random number from hardware
    static std::mt19937 gen(rd()); // seed the generator
    static std::uniform_int_distribution<> distr(0, 99);

    gen.seed(chunkPosition[0] << 8 | chunkPosition[1] << 16 | chunkPosition[2] << 24);

    // std::array<std::array<std::array<Block, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> this->blocks;
    for (int x = 0; x < Chunk::CHUNK_SIZE; x++){
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
                glm::ivec3 blockPosition(chunkPosition * (int) CHUNK_SIZE + glm::ivec3(x, y, z));

                Block::Type blockType = Block::Type::AIR;
                if (distr(gen) < 5) {
                    blockType = Block::Type::SOLID;
                }
                blocks[x][y][z].type = blockType;
                blocks[x][y][z].position = blockPosition;
                
            }
        }
    }
}

void Chunk::createVAO() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * vertexPositions.size() * sizeof(GLbyte), &vertexPositions[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 0, NULL);

    glGenBuffers(1, &textureCoordinatesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordinatesBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * textureCoordinates.size() * sizeof(GLbyte), &textureCoordinates[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_BYTE, GL_FALSE, 0, NULL);
}

void Chunk::createMesh() {
    this->vertexPositions = std::vector<glm::vec<3, GLubyte, glm::packed_highp>>();
    this->vertexColors = std::vector<glm::vec3>();
    this->textureCoordinates = std::vector<glm::vec<2, GLubyte, glm::packed_highp>>();

    // iterates over all blocks
    for (int x = 0; x < Chunk::CHUNK_SIZE; x++){
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
                if (this->blocks[x][y][z].type == Block::Type::AIR) {
                    continue;
                }
                glm::vec3 currentBlockPosition(x, y, z);

                if (
                (x < Chunk::CHUNK_SIZE-1 &&
                this->blocks[x+1][y][z].type == Block::Type::AIR) || 
                (x == Chunk::CHUNK_SIZE-1 && chunks.find(chunkPosition + glm::ivec3(1, 0, 0)) == chunks.end()) ||
                (x == Chunk::CHUNK_SIZE-1 && chunks.find(chunkPosition + glm::ivec3(1, 0, 0)) != chunks.end() && chunks.at(chunkPosition + glm::ivec3(1, 0, 0))->blocks[0][y][z].type == Block::Type::AIR)
                ) {
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

                if ((x > 0 &&
                this->blocks[x-1][y][z].type == Block::Type::AIR) || 
                (x == 0 && chunks.find(chunkPosition + glm::ivec3(-1, 0, 0)) == chunks.end()) ||
                (x == 0 && chunks.find(chunkPosition + glm::ivec3(-1, 0, 0)) != chunks.end() && chunks.at(chunkPosition + glm::ivec3(-1, 0, 0))->blocks[Chunk::CHUNK_SIZE-1][y][z].type == Block::Type::AIR)
                ) {
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

                if ((z < Chunk::CHUNK_SIZE-1 &&
                this->blocks[x][y][z+1].type == Block::Type::AIR) || 
                (z == Chunk::CHUNK_SIZE-1 && chunks.find(chunkPosition + glm::ivec3(0, 0, 1)) == chunks.end()) ||
                (z == Chunk::CHUNK_SIZE-1 && chunks.find(chunkPosition + glm::ivec3(0, 0, 1)) != chunks.end() && chunks.at(chunkPosition + glm::ivec3(0, 0, 1))->blocks[x][y][0].type == Block::Type::AIR)
                ) {
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

                if ((z > 0 &&
                this->blocks[x][y][z-1].type == Block::Type::AIR) || 
                (z == 0 && chunks.find(chunkPosition + glm::ivec3(0, 0, -1)) == chunks.end()) ||
                (z == 0 && chunks.find(chunkPosition + glm::ivec3(0, 0, -1)) != chunks.end() && chunks.at(chunkPosition + glm::ivec3(0, 0, -1))->blocks[x][y][Chunk::CHUNK_SIZE-1].type == Block::Type::AIR)
                ) {
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

                if ((y < Chunk::CHUNK_SIZE-1 &&
                this->blocks[x][y+1][z].type == Block::Type::AIR) || 
                (y == Chunk::CHUNK_SIZE-1 && chunks.find(chunkPosition + glm::ivec3(0, 1, 0)) == chunks.end()) ||
                (y == Chunk::CHUNK_SIZE-1 && chunks.find(chunkPosition + glm::ivec3(0, 1, 0)) != chunks.end() && chunks.at(chunkPosition + glm::ivec3(0, 1, 0))->blocks[x][0][z].type == Block::Type::AIR)
                ) {
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

                if ((y > 0 &&
                this->blocks[x][y-1][z].type == Block::Type::AIR) || 
                (y == 0 && chunks.find(chunkPosition + glm::ivec3(0, -1, 0)) == chunks.end()) ||
                (y == 0 && chunks.find(chunkPosition + glm::ivec3(0, -1, 0)) != chunks.end() && chunks.at(chunkPosition + glm::ivec3(0, -1, 0))->blocks[x][Chunk::CHUNK_SIZE-1][z].type == Block::Type::AIR)
                ) {
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