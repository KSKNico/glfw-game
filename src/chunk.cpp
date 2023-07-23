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
                glm::ivec3 blockPosition(position * (int)CHUNK_SIZE + glm::ivec3(x, y, z));

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

bool Chunk::isSurrounded() {
    for (int i = 0; i < 6; i++) {
        glm::ivec3 adjacentChunkPosition = this->position + Chunk::ADJACENT_CHUNK_POSITIONS[i];
        if (this->chunks.find(adjacentChunkPosition) == this->chunks.end()) {
            return false;
        }
    }
    return true;
}

// void Chunk::optimizeMesh() {
//     if (!this->isSurrounded()) {
//         return;
//     }
//     for (auto &vertex : this->vertexPositions) {
//         if (!isEdgeVertex(vertex)) {
//             continue;
//         }
//         for (int i = 0; i < 6; i++) {
//             glm::ivec3 adjacentChunkPosition = this->position + Chunk::ADJACENT_CHUNK_POSITIONS[i];
//             if (this->chunks.find(adjacentChunkPosition) == this->chunks.end()) {
//                 continue;
//             }
//             Chunk &adjacentChunk = *this->chunks[adjacentChunkPosition];
//             for (auto &adjacentVertex : adjacentChunk.vertexPositions) {
//                 if (adjacentVertex == vertex) {
//                     adjacentVertex = glm::vec3(0, 0, 0);
//                 }
//             }
//         }
//     }
// }

// bool Chunk::isVertexCongruent(const glm::vec3 vertex1, const glm::vec3 vertex2) {
//     return vertex1.x && vertex1.y == vertex2.y && vertex1.z == vertex2.z;
// }

void Chunk::createMesh() {
    this->vertices = std::vector<Vertex>();
    this->vertexCount = 0;

    // iterates over all blocks
    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
                if (this->blocks[x][y][z].type == Block::Type::AIR) {
                    continue;
                }
                v_position currentBlockPosition(x, y, z);

                if (
                    (x < Chunk::CHUNK_SIZE - 1 &&
                     this->blocks[x + 1][y][z].type == Block::Type::AIR) ||
                    (x == Chunk::CHUNK_SIZE - 1 && chunks.find(position + glm::ivec3(1, 0, 0)) == chunks.end()) ||
                    (x == Chunk::CHUNK_SIZE - 1 && chunks.find(position + glm::ivec3(1, 0, 0)) != chunks.end() && chunks.at(position + glm::ivec3(1, 0, 0))->blocks[0][y][z].type == Block::Type::AIR)) {
                    // right face
                    Vertex v1(currentBlockPosition + v_position(1, 0, 1), v_textureCoordinates(0, 1), 0, this->blocks[x][y][z].type - 1);
                    Vertex v2(currentBlockPosition + v_position(1, 0, 0), v_textureCoordinates(1, 1), 0, this->blocks[x][y][z].type - 1);
                    Vertex v3(currentBlockPosition + v_position(1, 1, 1), v_textureCoordinates(0, 0), 0, this->blocks[x][y][z].type - 1);
                    Vertex v4(currentBlockPosition + v_position(1, 1, 1), v_textureCoordinates(0, 0), 0, this->blocks[x][y][z].type - 1);
                    Vertex v5(currentBlockPosition + v_position(1, 0, 0), v_textureCoordinates(1, 1), 0, this->blocks[x][y][z].type - 1);
                    Vertex v6(currentBlockPosition + v_position(1, 1, 0), v_textureCoordinates(1, 0), 0, this->blocks[x][y][z].type - 1);

                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);
                    vertices.push_back(v5);
                    vertices.push_back(v6);

                    vertexCount += 6;
                }

                if ((x > 0 &&
                     this->blocks[x - 1][y][z].type == Block::Type::AIR) ||
                    (x == 0 && chunks.find(position + glm::ivec3(-1, 0, 0)) == chunks.end()) ||
                    (x == 0 && chunks.find(position + glm::ivec3(-1, 0, 0)) != chunks.end() && chunks.at(position + glm::ivec3(-1, 0, 0))->blocks[Chunk::CHUNK_SIZE - 1][y][z].type == Block::Type::AIR)) {
                    // left face
                    Vertex v1(currentBlockPosition + v_position(0, 0, 0), v_textureCoordinates(0, 1), 0, this->blocks[x][y][z].type - 1);
                    Vertex v2(currentBlockPosition + v_position(0, 0, 1), v_textureCoordinates(1, 1), 0, this->blocks[x][y][z].type - 1);
                    Vertex v3(currentBlockPosition + v_position(0, 1, 0), v_textureCoordinates(0, 0), 0, this->blocks[x][y][z].type - 1);
                    Vertex v4(currentBlockPosition + v_position(0, 1, 0), v_textureCoordinates(0, 0), 0, this->blocks[x][y][z].type - 1);
                    Vertex v5(currentBlockPosition + v_position(0, 0, 1), v_textureCoordinates(1, 1), 0, this->blocks[x][y][z].type - 1);
                    Vertex v6(currentBlockPosition + v_position(0, 1, 1), v_textureCoordinates(1, 0), 0, this->blocks[x][y][z].type - 1);

                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);
                    vertices.push_back(v5);
                    vertices.push_back(v6);

                    vertexCount += 6;
                }

                if ((z < Chunk::CHUNK_SIZE - 1 &&
                     this->blocks[x][y][z + 1].type == Block::Type::AIR) ||
                    (z == Chunk::CHUNK_SIZE - 1 && chunks.find(position + glm::ivec3(0, 0, 1)) == chunks.end()) ||
                    (z == Chunk::CHUNK_SIZE - 1 && chunks.find(position + glm::ivec3(0, 0, 1)) != chunks.end() && chunks.at(position + glm::ivec3(0, 0, 1))->blocks[x][y][0].type == Block::Type::AIR)) {
                    // front face
                    Vertex v1(currentBlockPosition + v_position(0, 0, 1), v_textureCoordinates(0, 1), 2, this->blocks[x][y][z].type - 1);
                    Vertex v2(currentBlockPosition + v_position(1, 0, 1), v_textureCoordinates(1, 1), 2, this->blocks[x][y][z].type - 1);
                    Vertex v3(currentBlockPosition + v_position(0, 1, 1), v_textureCoordinates(0, 0), 2, this->blocks[x][y][z].type - 1);
                    Vertex v4(currentBlockPosition + v_position(0, 1, 1), v_textureCoordinates(0, 0), 2, this->blocks[x][y][z].type - 1);
                    Vertex v5(currentBlockPosition + v_position(1, 0, 1), v_textureCoordinates(1, 1), 2, this->blocks[x][y][z].type - 1);
                    Vertex v6(currentBlockPosition + v_position(1, 1, 1), v_textureCoordinates(1, 0), 2, this->blocks[x][y][z].type - 1);

                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);
                    vertices.push_back(v5);
                    vertices.push_back(v6);

                    vertexCount += 6;
                }

                if ((z > 0 &&
                     this->blocks[x][y][z - 1].type == Block::Type::AIR) ||
                    (z == 0 && chunks.find(position + glm::ivec3(0, 0, -1)) == chunks.end()) ||
                    (z == 0 && chunks.find(position + glm::ivec3(0, 0, -1)) != chunks.end() && chunks.at(position + glm::ivec3(0, 0, -1))->blocks[x][y][Chunk::CHUNK_SIZE - 1].type == Block::Type::AIR)) {
                    // back face
                    Vertex v1(currentBlockPosition + v_position(1, 0, 0), v_textureCoordinates(0, 1), 2, this->blocks[x][y][z].type - 1);
                    Vertex v2(currentBlockPosition + v_position(0, 0, 0), v_textureCoordinates(1, 1), 2, this->blocks[x][y][z].type - 1);
                    Vertex v3(currentBlockPosition + v_position(1, 1, 0), v_textureCoordinates(0, 0), 2, this->blocks[x][y][z].type - 1);
                    Vertex v4(currentBlockPosition + v_position(1, 1, 0), v_textureCoordinates(0, 0), 2, this->blocks[x][y][z].type - 1);
                    Vertex v5(currentBlockPosition + v_position(0, 0, 0), v_textureCoordinates(1, 1), 2, this->blocks[x][y][z].type - 1);
                    Vertex v6(currentBlockPosition + v_position(0, 1, 0), v_textureCoordinates(1, 0), 2, this->blocks[x][y][z].type - 1);

                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);
                    vertices.push_back(v5);
                    vertices.push_back(v6);

                    vertexCount += 6;
                }

                if ((y < Chunk::CHUNK_SIZE - 1 &&
                     this->blocks[x][y + 1][z].type == Block::Type::AIR) ||
                    (y == Chunk::CHUNK_SIZE - 1 && chunks.find(position + glm::ivec3(0, 1, 0)) == chunks.end()) ||
                    (y == Chunk::CHUNK_SIZE - 1 && chunks.find(position + glm::ivec3(0, 1, 0)) != chunks.end() && chunks.at(position + glm::ivec3(0, 1, 0))->blocks[x][0][z].type == Block::Type::AIR)) {
                    // top face
                    Vertex v1(currentBlockPosition + v_position(0, 1, 1), v_textureCoordinates(0, 1), 1, this->blocks[x][y][z].type - 1);
                    Vertex v2(currentBlockPosition + v_position(1, 1, 1), v_textureCoordinates(1, 1), 1, this->blocks[x][y][z].type - 1);
                    Vertex v3(currentBlockPosition + v_position(0, 1, 0), v_textureCoordinates(0, 0), 1, this->blocks[x][y][z].type - 1);
                    Vertex v4(currentBlockPosition + v_position(0, 1, 0), v_textureCoordinates(0, 0), 1, this->blocks[x][y][z].type - 1);
                    Vertex v5(currentBlockPosition + v_position(1, 1, 1), v_textureCoordinates(1, 1), 1, this->blocks[x][y][z].type - 1);
                    Vertex v6(currentBlockPosition + v_position(1, 1, 0), v_textureCoordinates(1, 0), 1, this->blocks[x][y][z].type - 1);

                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);
                    vertices.push_back(v5);
                    vertices.push_back(v6);

                    vertexCount += 6;
                }

                if ((y > 0 &&
                     this->blocks[x][y - 1][z].type == Block::Type::AIR) ||
                    (y == 0 && chunks.find(position + glm::ivec3(0, -1, 0)) == chunks.end()) ||
                    (y == 0 && chunks.find(position + glm::ivec3(0, -1, 0)) != chunks.end() && chunks.at(position + glm::ivec3(0, -1, 0))->blocks[x][Chunk::CHUNK_SIZE - 1][z].type == Block::Type::AIR)) {
                    // bottom face
                    Vertex v1(currentBlockPosition + v_position(1, 0, 1), v_textureCoordinates(0, 1), 1, this->blocks[x][y][z].type - 1);
                    Vertex v2(currentBlockPosition + v_position(0, 0, 1), v_textureCoordinates(1, 1), 1, this->blocks[x][y][z].type - 1);
                    Vertex v3(currentBlockPosition + v_position(1, 0, 0), v_textureCoordinates(0, 0), 1, this->blocks[x][y][z].type - 1);
                    Vertex v4(currentBlockPosition + v_position(1, 0, 0), v_textureCoordinates(0, 0), 1, this->blocks[x][y][z].type - 1);
                    Vertex v5(currentBlockPosition + v_position(0, 0, 1), v_textureCoordinates(1, 1), 1, this->blocks[x][y][z].type - 1);
                    Vertex v6(currentBlockPosition + v_position(0, 0, 0), v_textureCoordinates(1, 0), 1, this->blocks[x][y][z].type - 1);

                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);
                    vertices.push_back(v5);
                    vertices.push_back(v6);

                    vertexCount += 6;
                }
            }
        }
    }
}