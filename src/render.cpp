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
    // TODO: this might be redundant
    if (!chunk.hasVAO && chunk.hasMesh) {
        chunk.createVAO();
    }

    world.chunkMutex.lock();

    glBindVertexArray(chunk.vao);
    glBindTexture(GL_TEXTURE_2D_ARRAY, blockTextures.id);

    glm::mat4 translationMatrix = glm::translate(chunk.position * (int)Chunk::CHUNK_SIZE);

    glm::mat4 MVP = viewProjectionMatrix * translationMatrix;

    GLint uniformMVP = blockShader.getUniformLocation("MVP");
    glUniformMatrix4fv(uniformMVP, 1, false, &MVP[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, chunk.vertexCount);

    world.chunkMutex.unlock();
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

        drawChunk(chunk, VP);
    }
}

void Renderer::render() {
    drawSkybox();
    drawBlocks();
}
