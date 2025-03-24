#include "chunk.h"

Chunk::Chunk(const glm::ivec3 &position, std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IntegerVec3Hasher> &chunks, unsigned int worldSeed, std::mutex &chunkMutex) : position(position), chunks(chunks), worldSeed(worldSeed), chunkMutex(chunkMutex) {
    // std::cout << "Created chunk at position " << position[0] << " " << position[1] << " " << position[2] << std::endl;
    this->populateChunk();
    this->createMesh();

    chunkVertices[0] = position * (int)Chunk::CHUNK_SIZE;
    chunkVertices[1] = position * (int)Chunk::CHUNK_SIZE + glm::ivec3(Chunk::CHUNK_SIZE, 0, 0);
    chunkVertices[2] = position * (int)Chunk::CHUNK_SIZE + glm::ivec3(Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0);
    chunkVertices[3] = position * (int)Chunk::CHUNK_SIZE + glm::ivec3(0, Chunk::CHUNK_SIZE, 0);
    chunkVertices[4] = position * (int)Chunk::CHUNK_SIZE + glm::ivec3(0, 0, Chunk::CHUNK_SIZE);
    chunkVertices[5] = position * (int)Chunk::CHUNK_SIZE + glm::ivec3(Chunk::CHUNK_SIZE, 0, Chunk::CHUNK_SIZE);
    chunkVertices[6] = position * (int)Chunk::CHUNK_SIZE + glm::ivec3(Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE);
    chunkVertices[7] = position * (int)Chunk::CHUNK_SIZE + glm::ivec3(0, Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE);
}

Chunk::~Chunk() {
    // free resources used on the GPU
    // std::cout << "Deleted chunk at position " << position[0] << " " << position[1] << " " << position[2] << std::endl;
    // TODO: this can create a segfault if the chunk is rendered while being deleted
}

glm::ivec3 Chunk::convertChunkToWorldCoordinates(const glm::ivec3 &blockPositionInChunk) {
    assert(isInside(blockPositionInChunk));

    return position * (int)Chunk::CHUNK_SIZE + blockPositionInChunk;
}

void Chunk::populateChunk() {
    static std::random_device rd;   // obtain a random number from hardware
    static std::mt19937 gen(rd());  // seed the generator
    static std::uniform_int_distribution<> distr(0, 99);

    gen.seed(position[0] << 8 | position[1] << 16 | position[2] << 24);
    const float threshold = 0.55f;
    const siv::PerlinNoise perlin_1{worldSeed};
    const siv::PerlinNoise perlin_2{worldSeed + 10000};

    // std::array<std::array<std::array<Block, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> this->blocks;
    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
                // block position is in world coordinates
                auto blockPosition = convertChunkToWorldCoordinates(glm::ivec3(x, y, z));

                Block::Type blockType = Block::Type::AIR;

                const double noise_1 = perlin_1.octave3D_01(blockPosition.x * 0.01, blockPosition.y * 0.01, blockPosition.z * 0.01, 4);
                const double noise_2 = perlin_2.octave3D_01(blockPosition.x * 0.01, blockPosition.y * 0.01, blockPosition.z * 0.01, 4);
                if (noise_1 > threshold) {
                    blockType = Block::Type::RED_STONE;
                } else if (noise_2 > threshold * 1.1f) {
                    blockType = Block::Type::PURPLE_STONE;
                }
                blocks[x][y][z].type = blockType;
                blocks[x][y][z].position = blockPosition;
            }
        }
    }
}

bool Chunk::isInside(const glm::ivec3 &blockPositionInChunk) {
    return blockPositionInChunk.x >= 0 && blockPositionInChunk.x < Chunk::CHUNK_SIZE && blockPositionInChunk.y >= 0 && blockPositionInChunk.y < Chunk::CHUNK_SIZE && blockPositionInChunk.z >= 0 && blockPositionInChunk.z < Chunk::CHUNK_SIZE;
}

bool Chunk::isVisible(const Block &block, Direction direction) {
    // air is not rendered at all
    if (block.type == Block::Type::AIR) {
        return false;
    }

    // if it is inside the chunk, check if the other block is air
    auto otherBlockPosition = block.position + DIRECTION_VECTORS[static_cast<int>(direction)];
    if (isInside(otherBlockPosition)) {
        // the simple case where can simply look at the other block because we stay in the same chunk
        return this->blocks[otherBlockPosition.x][otherBlockPosition.y][otherBlockPosition.z].type == Block::Type::AIR;
    } else {
        // we need to access the other chunk
        auto otherChunkPosition = position + DIRECTION_VECTORS[static_cast<int>(direction)];
        otherBlockPosition.x = otherBlockPosition.x % Chunk::CHUNK_SIZE;
        otherBlockPosition.y = otherBlockPosition.y % Chunk::CHUNK_SIZE;
        otherBlockPosition.z = otherBlockPosition.z % Chunk::CHUNK_SIZE;
        if (chunks.find(otherChunkPosition) == chunks.end()) {
            return true;
        }
        return chunks.at(otherChunkPosition)->blocks[otherBlockPosition.x][otherBlockPosition.y][otherBlockPosition.z].type == Block::Type::AIR;
    }
}

Block Chunk::getBlockBySlice(int s, int i, int k, Direction direction) {
    if (direction == Direction::POS_X || direction == Direction::NEG_X) {
        return blocks[s][i][k];
    } else if (direction == Direction::POS_Y || direction == Direction::NEG_Y) {
        return blocks[i][s][k];
    } else {
        return blocks[i][k][s];
    }
}

// returns true if the position is inside the quad
static bool isPositionInQuad(const Chunk::quad q, const glm::u8vec2 p) {
    // simply check if the position p is inside the quad q
    return p.x >= q.first.x && p.x <= q.second.x && p.y >= q.first.y && p.y <= q.second.y;
}

// returns true if the position of the slice has already been meshed
static bool isPositionMeshed(Chunk::quadMesh &quads, int i, int k) {
    for (auto &quad : quads) {
        if (isPositionInQuad(quad, {i, k})) {
            return true;
        }
    }
    return false;
}

Chunk::quadMesh Chunk::greedyMeshing(unsigned int sliceIndex, Direction direction) {
    // start with the lowest y coordinate first and expand to the right (increase x)
    // if you hit a block that can't be included in the quad, move in y direction
    quadMesh quads;

    auto currentX = 0;
    auto currentY = 0;
    auto currentType = Block::Type::AIR;

    while (true) {
        auto found = false;
        // try to find a quad to expand from
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int i = 0; i < CHUNK_SIZE; i++) {
                if (isPositionMeshed(quads, i, j)) {
                    continue;
                }

                currentX = i;
                currentY = j;
                found = false;
                break;
            }
        }

        if (!found) {
            break;
        }

        auto block = getBlockBySlice(sliceIndex, currentX, currentY, direction);
        currentType = block.type;
        if (!isVisible(block, direction)) {
            continue;
        }

        // find the width of the quad
        auto width = 1;
        while (currentX + width < Chunk::CHUNK_SIZE) {
            auto otherBlock = getBlockBySlice(sliceIndex, currentX + width, currentY, direction);
            if (!isVisible(otherBlock, direction) || otherBlock.type != currentType || isPositionMeshed(quads, currentX + width, currentY)) {
                break;
            }
            width++;
        }

        // find the height of the quad
        auto height = 1;
        while (currentY + height < Chunk::CHUNK_SIZE) {
            bool canExpand = true;
            for (int i = 0; i < width; i++) {
                auto otherBlock = getBlockBySlice(sliceIndex, currentX + i, currentY + height, direction);
                if (!isVisible(otherBlock, direction) || otherBlock.type != currentType || isPositionMeshed(quads, currentX + i, currentY + height)) {
                    canExpand = false;
                    break;
                }
            }
            if (!canExpand) {
                break;
            }
            height++;
        }

        // add the quad to the list
        quads.push_back(quad({currentX, currentY}, {currentX + width - 1, currentY + height - 1}));
    }
    return quads;
}

void Chunk::createMesh() {
    this->vertexPositions = std::vector<glm::vec<3, GLubyte, glm::packed_highp>>();
    this->vertexColors = std::vector<glm::vec3>();
    this->textureCoordinates = std::vector<glm::vec<2, GLubyte, glm::packed_highp>>();
    this->vertexFacing = std::vector<GLubyte>();
    this->textureIndices = std::vector<GLubyte>();
    this->vertexCount = 0;

    // iterates over all blocks
    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
                if (this->blocks[x][y][z].type == Block::Type::AIR) {
                    continue;
                }
                glm::vec3 currentBlockPosition(x, y, z);

                if (
                    (x < Chunk::CHUNK_SIZE - 1 &&
                     this->blocks[x + 1][y][z].type == Block::Type::AIR) ||
                    (x == Chunk::CHUNK_SIZE - 1 && chunks.find(position + glm::ivec3(1, 0, 0)) == chunks.end()) ||
                    (x == Chunk::CHUNK_SIZE - 1 && chunks.find(position + glm::ivec3(1, 0, 0)) != chunks.end() && chunks.at(position + glm::ivec3(1, 0, 0))->blocks[0][y][z].type == Block::Type::AIR)) {
                    // right face
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 0, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 0, 0));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 1, 1));

                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 1, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 0, 0));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 1, 0));

                    /* vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); */

                    textureCoordinates.push_back(glm::vec2(0, 1));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(0, 0));

                    textureCoordinates.push_back(glm::vec2(0, 0));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(1, 0));

                    vertexFacing.push_back(0);
                    vertexFacing.push_back(0);
                    vertexFacing.push_back(0);

                    vertexFacing.push_back(0);
                    vertexFacing.push_back(0);
                    vertexFacing.push_back(0);

                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);

                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);

                    vertexCount += 6;
                }

                if ((x > 0 &&
                     this->blocks[x - 1][y][z].type == Block::Type::AIR) ||
                    (x == 0 && chunks.find(position + glm::ivec3(-1, 0, 0)) == chunks.end()) ||
                    (x == 0 && chunks.find(position + glm::ivec3(-1, 0, 0)) != chunks.end() && chunks.at(position + glm::ivec3(-1, 0, 0))->blocks[Chunk::CHUNK_SIZE - 1][y][z].type == Block::Type::AIR)) {
                    // left face
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 0, 0));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 0, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 1, 0));

                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 1, 0));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 0, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 1, 1));

                    /* vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); */

                    textureCoordinates.push_back(glm::vec2(0, 1));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(0, 0));

                    textureCoordinates.push_back(glm::vec2(0, 0));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(1, 0));

                    vertexFacing.push_back(0);
                    vertexFacing.push_back(0);
                    vertexFacing.push_back(0);

                    vertexFacing.push_back(0);
                    vertexFacing.push_back(0);
                    vertexFacing.push_back(0);

                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);

                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);

                    vertexCount += 6;
                }

                if ((z < Chunk::CHUNK_SIZE - 1 &&
                     this->blocks[x][y][z + 1].type == Block::Type::AIR) ||
                    (z == Chunk::CHUNK_SIZE - 1 && chunks.find(position + glm::ivec3(0, 0, 1)) == chunks.end()) ||
                    (z == Chunk::CHUNK_SIZE - 1 && chunks.find(position + glm::ivec3(0, 0, 1)) != chunks.end() && chunks.at(position + glm::ivec3(0, 0, 1))->blocks[x][y][0].type == Block::Type::AIR)) {
                    // front face
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 0, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 0, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 1, 1));

                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 1, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 0, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 1, 1));

                    /* vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); */

                    textureCoordinates.push_back(glm::vec2(0, 1));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(0, 0));

                    textureCoordinates.push_back(glm::vec2(0, 0));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(1, 0));

                    vertexFacing.push_back(2);
                    vertexFacing.push_back(2);
                    vertexFacing.push_back(2);

                    vertexFacing.push_back(2);
                    vertexFacing.push_back(2);
                    vertexFacing.push_back(2);

                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);

                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);

                    vertexCount += 6;
                }

                if ((z > 0 &&
                     this->blocks[x][y][z - 1].type == Block::Type::AIR) ||
                    (z == 0 && chunks.find(position + glm::ivec3(0, 0, -1)) == chunks.end()) ||
                    (z == 0 && chunks.find(position + glm::ivec3(0, 0, -1)) != chunks.end() && chunks.at(position + glm::ivec3(0, 0, -1))->blocks[x][y][Chunk::CHUNK_SIZE - 1].type == Block::Type::AIR)) {
                    // back face
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 0, 0));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 0, 0));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 1, 0));

                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 1, 0));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 0, 0));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 1, 0));

                    /* vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); */

                    textureCoordinates.push_back(glm::vec2(0, 1));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(0, 0));

                    textureCoordinates.push_back(glm::vec2(0, 0));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(1, 0));

                    vertexFacing.push_back(2);
                    vertexFacing.push_back(2);
                    vertexFacing.push_back(2);

                    vertexFacing.push_back(2);
                    vertexFacing.push_back(2);
                    vertexFacing.push_back(2);

                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);

                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);

                    vertexCount += 6;
                }

                if ((y < Chunk::CHUNK_SIZE - 1 &&
                     this->blocks[x][y + 1][z].type == Block::Type::AIR) ||
                    (y == Chunk::CHUNK_SIZE - 1 && chunks.find(position + glm::ivec3(0, 1, 0)) == chunks.end()) ||
                    (y == Chunk::CHUNK_SIZE - 1 && chunks.find(position + glm::ivec3(0, 1, 0)) != chunks.end() && chunks.at(position + glm::ivec3(0, 1, 0))->blocks[x][0][z].type == Block::Type::AIR)) {
                    // top face
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 1, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 1, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 1, 0));

                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 1, 0));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 1, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 1, 0));

                    /* vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); */

                    textureCoordinates.push_back(glm::vec2(0, 1));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(0, 0));

                    textureCoordinates.push_back(glm::vec2(0, 0));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(1, 0));

                    vertexFacing.push_back(1);
                    vertexFacing.push_back(1);
                    vertexFacing.push_back(1);

                    vertexFacing.push_back(1);
                    vertexFacing.push_back(1);
                    vertexFacing.push_back(1);

                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);

                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);

                    vertexCount += 6;
                }

                if ((y > 0 &&
                     this->blocks[x][y - 1][z].type == Block::Type::AIR) ||
                    (y == 0 && chunks.find(position + glm::ivec3(0, -1, 0)) == chunks.end()) ||
                    (y == 0 && chunks.find(position + glm::ivec3(0, -1, 0)) != chunks.end() && chunks.at(position + glm::ivec3(0, -1, 0))->blocks[x][Chunk::CHUNK_SIZE - 1][z].type == Block::Type::AIR)) {
                    // bottom face
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 0, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 0, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 0, 0));

                    vertexPositions.push_back(currentBlockPosition + glm::vec3(1, 0, 0));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 0, 1));
                    vertexPositions.push_back(currentBlockPosition + glm::vec3(0, 0, 0));

                    /* vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    vertexColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    vertexColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); */

                    textureCoordinates.push_back(glm::vec2(0, 1));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(0, 0));

                    textureCoordinates.push_back(glm::vec2(0, 0));
                    textureCoordinates.push_back(glm::vec2(1, 1));
                    textureCoordinates.push_back(glm::vec2(1, 0));

                    vertexFacing.push_back(1);
                    vertexFacing.push_back(1);
                    vertexFacing.push_back(1);

                    vertexFacing.push_back(1);
                    vertexFacing.push_back(1);
                    vertexFacing.push_back(1);

                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);

                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);
                    textureIndices.push_back(this->blocks[x][y][z].type - 1);

                    vertexCount += 6;
                }
            }
        }
    }
}