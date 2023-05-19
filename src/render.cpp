#include "render.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Renderer::Renderer(GLFWwindow& window, Camera& camera, World& world) : window(window), camera(camera), world(world), skyboxTexture("skybox"),
blockTexture("block_texture"), blockShader("lighting_vertex_shader", "lighting_fragment_shader"), skyboxShader("skybox_vertex_shader", "skybox_fragment_shader")  {
  perspectiveMatrix = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.f);
}

void Renderer::setPerspectiveMatrix(int width, int height) {
  if (height == 0) {
    height = 1;
  }
  perspectiveMatrix = glm::perspective(45.0f, (float) width / height, 0.1f, 1000.f);
}


void Renderer::init() {
    // Shader("lighting_vertex_shader", "lighting_fragment_shader").id;
    // skyboxShader = Shader("skybox_vertex_shader", "skybox_fragment_shader").id;

    //blockTexture = loadBlockTexture("block_texture");
    //skyboxTexture = CubemapTexture("skybox");

}

void Renderer::render() {
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


  // render the blocks 
  blockShader.use();
  glDepthFunc(GL_LESS);

  uniformMVP = glGetUniformLocation(blockShader.id, "MVP");
  GLint uniformCameraVector = glGetUniformLocation(blockShader.id, "cameraVector");
  // VP = this->perspectiveMatrix * camera.getCameraMatrix();

  // glm::mat4 skyboxMatrix = VP * glm::translate(camera.position) * glm::scale(glm::vec3(1000.0f, 1000.0f, 1000.0f));
  glUniform3fv(uniformCameraVector, 1, &camera.lookAtDirection[0]);

  VP = this->perspectiveMatrix * camera.getCameraMatrix();
  for (auto& chunkPair : world.chunks)
  {
    if (!camera.isChunkInView(*(chunkPair.second))) {
      continue;
    }
    if (!chunkPair.second->hasVAO && chunkPair.second->hasMesh) {
      chunkPair.second->createVAO();
    } 
    glBindVertexArray(chunkPair.second->vao);
    glBindTexture(GL_TEXTURE_2D, blockTexture.id);

    glm::mat4 translationMatrix = glm::translate(chunkPair.second->position * (int) Chunk::CHUNK_SIZE);


    glm::mat4 MVP = VP * translationMatrix;

    glUniformMatrix4fv(uniformMVP, 1, false, &MVP[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, chunkPair.second->vertexCount);
  }
}
