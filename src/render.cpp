#include "render.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Renderer::Renderer(GLFWwindow& window, Camera& camera, World& world) : window(window), camera(camera), world(world), skyboxTexture("skybox"), blockTextures("blocks"), blockShader("lighting_array_vertex_shader", "lighting_array_fragment_shader"), skyboxShader("skybox_vertex_shader", "skybox_fragment_shader") {
    perspectiveMatrix = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.f);
}

void Renderer::setPerspectiveMatrix(int width, int height) {
    if (height == 0) {
        height = 1;
    }
    perspectiveMatrix = glm::perspective(45.0f, (float)width / height, 0.1f, 1000.f);
}

void Renderer::init() {
    GLuint samplerLocation = glGetUniformLocation(this->blockShader.id, "ourTextures");
    glUniform1i(samplerLocation, 0);
}

// removes all VAOs from the map that are not in world.chunks
void Renderer::unloadChunkVAOs() {
    world.chunkMutex.lock();
    std::vector<glm::ivec3>
        toRemove;
    for (auto& [position, vao] : chunkVAOs) {
        if (world.chunks.find(position) == world.chunks.end()) {
            toRemove.push_back(position);
        }
    }
    for (auto& position : toRemove) {
        deleteChunkVAO(position);
    }
    world.chunkMutex.unlock();
}

void Renderer::deleteChunkVAO(glm::ivec3 position) {
    auto& [vao, vertexBuffer, textureCoordinatesBuffer, vertexFacingBuffer, textureIndicesBuffer] = chunkVAOs[position];
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &textureCoordinatesBuffer);
    glDeleteBuffers(1, &vertexFacingBuffer);
    glDeleteBuffers(1, &textureIndicesBuffer);

    // delete vao from map
    chunkVAOs.erase(position);
}

void Renderer::createChunkVAO(Chunk& chunk) {
    std::array<GLuint, 5> buffers;

    glGenVertexArrays(1, &buffers[0]);
    glBindVertexArray(buffers[0]);

    glGenBuffers(1, &buffers[1]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * chunk.vertexPositions.size() * sizeof(GLubyte), &chunk.vertexPositions[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, NULL);

    glGenBuffers(1, &buffers[2]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, 2 * chunk.textureCoordinates.size() * sizeof(GLubyte), &chunk.textureCoordinates[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, 0, NULL);

    glGenBuffers(1, &buffers[3]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ARRAY_BUFFER, chunk.vertexFacing.size() * sizeof(GLubyte), &chunk.vertexFacing[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, 0, NULL);

    glGenBuffers(1, &buffers[4]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
    glBufferData(GL_ARRAY_BUFFER, chunk.textureIndices.size() * sizeof(GLubyte), &chunk.textureIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, 0, NULL);

    // add it to the VAO map, use copy operations
    chunkVAOs[chunk.position] = buffers;
}

void Renderer::drawSkybox() {
    // render the skybox
    skyboxShader.use();
    glDepthFunc(GL_LEQUAL);

    glBindVertexArray(world.skybox.vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture.id);

    GLint uniformMVP = glGetUniformLocation(skyboxShader.id, "MVP");
    glm::mat4 VP = this->perspectiveMatrix * camera.getCameraMatrix();
    glm::mat4 MVP = VP * glm::translate(camera.position);
    glUniformMatrix4fv(uniformMVP, 1, false, &MVP[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, world.skybox.vertexPositions.size());
}

void Renderer::drawChunk(Chunk& chunk, const glm::mat4& viewProjectionMatrix) {
    glBindVertexArray(chunkVAOs[chunk.position][0]);
    glBindTexture(GL_TEXTURE_2D_ARRAY, blockTextures.id);

    glm::mat4 translationMatrix = glm::translate(chunk.position * (int)Chunk::CHUNK_SIZE);

    glm::mat4 MVP = viewProjectionMatrix * translationMatrix;

    GLint uniformMVP = blockShader.getUniformLocation("MVP");
    glUniformMatrix4fv(uniformMVP, 1, false, &MVP[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, chunk.vertexCount);
}

void Renderer::drawBlocks() {
    // render the blocks
    blockShader.use();
    glDepthFunc(GL_LESS);

    // VP = this->perspectiveMatrix * camera.getCameraMatrix();

    // glm::mat4 skyboxMatrix = VP * glm::translate(camera.position) * glm::scale(glm::vec3(1000.0f, 1000.0f, 1000.0f));
    GLint uniformCameraVector = blockShader.getUniformLocation("cameraVector");
    glUniform3fv(uniformCameraVector, 1, &camera.lookAtDirection[0]);

    glm::mat4 VP = this->perspectiveMatrix * camera.getCameraMatrix();
    for (auto& chunkPair : world.chunks) {
        Chunk& chunk = *(chunkPair.second);
        if (!camera.isChunkInView(chunk)) {
            continue;
        }
        // check if chunk exists in VAO map
        if (chunkVAOs.find(chunk.position) == chunkVAOs.end()) {
            createChunkVAO(chunk);
        }

        drawChunk(chunk, VP);
    }
    unloadChunkVAOs();
}

void Renderer::render() {
    drawSkybox();
    drawBlocks();
}
