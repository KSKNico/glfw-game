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
    for (auto& [position, buffers] : this->chunkBuffers) {
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
    auto& [vao, vertexBuffer] = chunkBuffers[position];
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vertexBuffer);

    // delete vao from map
    chunkBuffers.erase(position);
}

void Renderer::createChunkVAO(Chunk& chunk) {
    GLuint vao;
    GLuint vertexBuffer;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, chunk.vertices.size() * sizeof(std::array<GLuint, 2>), &Vertex::interlaceVertexData(chunk.vertices)[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_UNSIGNED_INT, GL_FALSE, 0, NULL);

    std::array<GLuint, 2> buffers = {vao, vertexBuffer};
    // add it to the VAO map, use copy operations
    chunkBuffers[chunk.position] = buffers;
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
    glBindVertexArray(chunkBuffers[chunk.position][0]);
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
        if (chunkBuffers.find(chunk.position) == chunkBuffers.end()) {
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
