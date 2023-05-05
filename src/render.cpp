#include "render.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Renderer::Renderer(GLFWwindow& window, Camera& camera, World& world) : window(window), camera(camera), world(world)   {
  perspectiveMatrix = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.f);
}

void Renderer::setPerspectiveMatrix(int width, int height) {
  if (height == 0) {
    height = 1;
  }
  perspectiveMatrix = glm::perspective(45.0f, (float) width / height, 0.1f, 1000.f);
}


void Renderer::init() {
    blockShader = Shader("lighting_vertex_shader", "lighting_fragment_shader").id;
    skyboxShader = Shader("skybox_vertex_shader", "skybox_fragment_shader").id;

    blockTexture = loadBlockTexture("uv_grid_opengl");
    skyboxTexture = loadCubeMapTexture("skybox");

}

void Renderer::render() {
  // render the skybox
  glUseProgram(skyboxShader);
  glDepthFunc(GL_LEQUAL);

  glBindVertexArray(world.skybox.vao);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

  GLint uniformMVP = glGetUniformLocation(skyboxShader, "MVP");
  glm::mat4 VP = this->perspectiveMatrix * camera.getCameraMatrix();
  glm::mat4 MVP = VP * glm::translate(camera.position);
  glUniformMatrix4fv(uniformMVP, 1, false, &MVP[0][0]);

  glDrawArrays(GL_TRIANGLES, 0, world.skybox.vertexPositions.size());


  // render the blocks 
  glUseProgram(blockShader);

  uniformMVP = glGetUniformLocation(blockShader, "MVP");
  GLint uniformCameraVector = glGetUniformLocation(blockShader, "cameraVector");
  // VP = this->perspectiveMatrix * camera.getCameraMatrix();

  // glm::mat4 skyboxMatrix = VP * glm::translate(camera.position) * glm::scale(glm::vec3(1000.0f, 1000.0f, 1000.0f));
  glUniform3fv(uniformCameraVector, 1, &camera.lookatDirection[0]);

  VP = this->perspectiveMatrix * camera.getCameraMatrix();
  for (auto& chunkPair : world.chunks)
  {
    glBindVertexArray(chunkPair.second->vao);
    glBindTexture(GL_TEXTURE_2D, blockTexture);

    glm::mat4 translationMatrix = glm::translate(chunkPair.second->chunkPosition * (int) Chunk::CHUNK_SIZE);


    glm::mat4 MVP = VP * translationMatrix;

    glUniformMatrix4fv(uniformMVP, 1, false, &MVP[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, chunkPair.second->vertexPositions.size());   
  }
}
