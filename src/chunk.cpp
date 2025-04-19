#include "chunk.h"

#include "exporter.h"

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

    // exportToOBJ(vertexPositions, indexBuffer, "chunk_" + std::to_string(position.x) + "_" + std::to_string(position.y) + "_" + std::to_string(position.z) + ".obj");
}

Chunk::~Chunk() {
    // free resources used on the GPU
    // std::cout << "Deleted chunk at position " << position[0] << " " << position[1] << " " << position[2] << std::endl;
    // TODO: this can create a segfault if the chunk is rendered while being deleted
}

worldCoordinates Chunk::convertChunkToWorldCoordinates(const localCoordinates &blockPositionInChunk) {
    return position * (int)Chunk::CHUNK_SIZE + (worldCoordinates) blockPositionInChunk;
}

void Chunk::populateChunk() {
    static_assert (CHUNK_SIZE <= 255, "CHUNK_SIZE must be smaller than 256");

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
                blocks[x][y][z].localPosition = glm::u8vec3(x, y, z);
            }
        }
    }
}

bool Chunk::isInside(const worldCoordinates &blockPositionInChunk) {
    return blockPositionInChunk.x >= 0 && blockPositionInChunk.x < Chunk::CHUNK_SIZE && blockPositionInChunk.y >= 0 && blockPositionInChunk.y < Chunk::CHUNK_SIZE && blockPositionInChunk.z >= 0 && blockPositionInChunk.z < Chunk::CHUNK_SIZE;
}

bool Chunk::isVisible(const Block &block, Direction direction) {
    // air is not rendered at all
    if (block.type == Block::Type::AIR) {
        return false;
    }

    // if it is inside the chunk, check if the other block is air
    auto otherBlockPosition = (worldCoordinates) block.localPosition + DIRECTION_VECTORS[static_cast<int>(direction)];
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

// filters out the direction from the position in the slice
static glm::u8vec2 get2DPositionInSlice(const glm::u8vec3 position, Direction direction) {
    // get the quad in the slice
    if (direction == Direction::POS_X || direction == Direction::NEG_X) {
        return {position.y, position.z};
    } else if (direction == Direction::POS_Y || direction == Direction::NEG_Y) {
        return {position.x, position.z};
    } else {
        return {position.x, position.y};
    }
}

// returns true if the position p is inside the quad q
// the quad has 2D coordinates unlike the quads used for rendering
static bool isPositionInQuad(const std::pair<glm::u8vec2, glm::u8vec2> q, const glm::u8vec2 p) {
    // simply check if the position p is inside the quad q
    return p.x >= q.first.x && p.x <= q.second.x && p.y >= q.first.y && p.y <= q.second.y;
}

// returns true if the position {i,k} of the slice has already been meshed
// this requires the direction from which the slice is viewed and the already meshed quads
static bool isPositionMeshed(Chunk::quadMesh &quads, int i, int k, Direction direction) {
    for (auto &quad : quads) {
        if (isPositionInQuad({get2DPositionInSlice(quad.first, direction), get2DPositionInSlice(quad.second, direction)}, {i, k})) {
            return true;
        }
    }
    return false;
}

Chunk::quadMesh Chunk::greedyMeshing(unsigned int sliceIndex, Direction direction) {
    // start with the lowest y coordinate first and expand to the right (increase x)
    // if you hit a block that can't be included in the quad, move in y direction
    Chunk::quadMesh quads;

    auto currentType = Block::Type::AIR;

    // try to find a quad to expand from
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            if (isPositionMeshed(quads, x, y, direction)) {
                continue;
            }

            auto block = getBlockBySlice(sliceIndex, x, y, direction);
            currentType = block.type;
            if (!isVisible(block, direction)) {
                continue;
            }
    
            // find the width of the quad
            auto width = 1;
            while (x + width < Chunk::CHUNK_SIZE) {
                auto otherBlock = getBlockBySlice(sliceIndex, x + width, y, direction);
                if (!isVisible(otherBlock, direction) ||
                    otherBlock.type != currentType ||
                    isPositionMeshed(quads, x + width, y, direction)) {
                    break;
                }
                width++;
            }
    
            // find the height of the quad
            auto height = 1;
            while (y + height < Chunk::CHUNK_SIZE) {
                bool canExpand = true;
                for (int i = 0; i < width; i++) {
                    auto otherBlock = getBlockBySlice(sliceIndex, x + i, y + height, direction);
                    if (!isVisible(otherBlock, direction) ||
                        otherBlock.type != currentType ||
                        isPositionMeshed(quads, x + i, y + height, direction)) {
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
            if (direction == Direction::POS_X) {
                quads.push_back(Quad({sliceIndex + 1, x, y}, {sliceIndex + 1, x + width, y + height}, currentType));
            } else if (direction == Direction::NEG_X) {
                quads.push_back(Quad({sliceIndex, x, y}, {sliceIndex, x + width, y + height}, currentType));
            } else if (direction == Direction::POS_Y) {
                quads.push_back(Quad({x, sliceIndex + 1, y}, {x + width, sliceIndex + 1, y + height}, currentType));
            } else if (direction == Direction::NEG_Y) {
                quads.push_back(Quad({x, sliceIndex, y}, {x + width, sliceIndex, y + height}, currentType));
            } else if (direction == Direction::POS_Z) {
                quads.push_back(Quad({x, y, sliceIndex + 1}, {x + width, y + height, sliceIndex + 1}, currentType));
            } else {
                quads.push_back(Quad({x, y, sliceIndex}, {x + width, y + height, sliceIndex}, currentType));
            }
        }
    }

    return quads;
}

void Chunk::createMesh() {
    // create the mesh for the chunk
    // this is a greedy meshing algorithm that creates quads for each block
    // and merges them into larger quads if possible

    for (size_t dir = 0; dir < (size_t)DIRECTION_VECTORS.size(); dir++) {
        auto direction = static_cast<Direction>(dir);
        for (int i = 0; i < Chunk::CHUNK_SIZE; i++) {
            for (int k = 0; k < Chunk::CHUNK_SIZE; k++) {
                auto slice = greedyMeshing(i, direction);
                for (auto &quad : slice) {
                    // add the quad to the mesh
                    /* auto block = getBlockBySlice(i, quad.first.x, quad.first.y, direction);
                    if (block.type == Block::Type::AIR) {
                        continue;
                    } */

                    if (direction == Direction::POS_X || direction == Direction::NEG_X) {
                        vertexPositions.push_back(glm::vec3(i, quad.first.y, quad.first.z));
                        vertexPositions.push_back(glm::vec3(i, quad.first.y, quad.second.z));
                        vertexPositions.push_back(glm::vec3(i, quad.second.y, quad.second.z));
                        vertexPositions.push_back(glm::vec3(i, quad.second.y, quad.first.z));
                    } else if (direction == Direction::POS_Y || direction == Direction::NEG_Y) {
                        vertexPositions.push_back(glm::vec3(quad.first.x, i, quad.first.z));
                        vertexPositions.push_back(glm::vec3(quad.first.x, i, quad.second.z));
                        vertexPositions.push_back(glm::vec3(quad.second.x, i, quad.second.z));
                        vertexPositions.push_back(glm::vec3(quad.second.x, i, quad.first.z));
                    } else {
                        vertexPositions.push_back(glm::vec3(quad.first.x, quad.first.y, i));
                        vertexPositions.push_back(glm::vec3(quad.first.x, quad.second.y, i));
                        vertexPositions.push_back(glm::vec3(quad.second.x, quad.second.y, i));
                        vertexPositions.push_back(glm::vec3(quad.second.x, quad.first.y, i));
                    }

                    textureIndices.push_back(quad.textureIndex);
                    textureIndices.push_back(quad.textureIndex);
                    textureIndices.push_back(quad.textureIndex);
                    textureIndices.push_back(quad.textureIndex);

                    vertexFacing.push_back(static_cast<GLubyte>(direction));
                    vertexFacing.push_back(static_cast<GLubyte>(direction));
                    vertexFacing.push_back(static_cast<GLubyte>(direction));
                    vertexFacing.push_back(static_cast<GLubyte>(direction));

                    /*

                        1--------2
                        |        |
                        |        |
                        |        |
                        |        |
                        0--------3

                    */
                    // first triangle CCW
                    // 0 3 1
                    indexBuffer.push_back(vertexPositions.size() - 4); // 0
                    indexBuffer.push_back(vertexPositions.size() - 1); // 3
                    indexBuffer.push_back(vertexPositions.size() - 3); // 1

                    // second triangle CCW
                    // 2 1 3
                    indexBuffer.push_back(vertexPositions.size() - 2); // 2
                    indexBuffer.push_back(vertexPositions.size() - 3); // 1
                    indexBuffer.push_back(vertexPositions.size() - 1); // 3

                    assert(textureIndices.size() == vertexPositions.size());
                    assert(vertexFacing.size() == vertexPositions.size());
                    assert(indexBuffer.size() % 6 == 0);
                }
            }
        }
    }
}