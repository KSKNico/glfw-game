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
    const std::string block_vertex_shader_text = loadShader("lighting_vertex_shader");
    const std::string block_fragment_shader_text = loadShader("lighting_fragment_shader");

    const std::string skybox_vertex_shader_text = loadShader("skybox_vertex_shader");
    const std::string skybox_fragment_shader_text = loadShader("skybox_fragment_shader");
    const GLchar* source;
    GLint isCompiled = 0;
    GLint program_linked;

    // Shaders
    source = (GLchar *) block_vertex_shader_text.c_str();
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &source, NULL);
    glCompileShader(vs);
    isCompiled = 0;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
      {
        GLint maxLength = 0;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vs, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the shader anymore.
        glDeleteShader(vs);

        for (auto c: infoLog) {
          std::cout << c;
        }
        return;
      }



    source = (GLchar *) block_fragment_shader_text.c_str();
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &source, NULL);
    glCompileShader(fs);
    isCompiled = 0;
    glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
      {
        GLint maxLength = 0;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fs, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the shader anymore.
        glDeleteShader(fs);

        for (auto c: infoLog) {
          std::cout << c;
        }
        
        return;
      }


    blockShader = glCreateProgram();
    glAttachShader(blockShader, fs);
    glAttachShader(blockShader, vs);
    glLinkProgram(blockShader);

    glGetProgramiv(blockShader, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
      GLsizei log_length = 0;
      GLchar message[1024];
      glGetProgramInfoLog(blockShader, 1024, &log_length, message);
      for (auto c: message) {
        std::cout << c;
      }


    }

    source = (GLchar *) skybox_vertex_shader_text.c_str();
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &source, NULL);
    glCompileShader(vs);
    isCompiled = 0;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
      {
        GLint maxLength = 0;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vs, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the shader anymore.
        glDeleteShader(vs);

        for (auto c: infoLog) {
          std::cout << c;
        }
        return;
      }

    source = (GLchar *) skybox_fragment_shader_text.c_str();
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &source, NULL);
    glCompileShader(fs);
    isCompiled = 0;
    glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
      {
        GLint maxLength = 0;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fs, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the shader anymore.
        glDeleteShader(fs);

        for (auto c: infoLog) {
          std::cout << c;
        }
        return;
      }




    skyboxShader = glCreateProgram();
    glAttachShader(skyboxShader, fs);
    glAttachShader(skyboxShader, vs);
    glLinkProgram(skyboxShader);
    glGetProgramiv(skyboxShader, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE) {
      GLsizei log_length = 0;
      GLchar message[1024];
      glGetProgramInfoLog(skyboxShader, 1024, &log_length, message);
      for (auto c: message) {
        std::cout << c;
      }
    }


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
  glm::mat4 MVP = VP;
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
