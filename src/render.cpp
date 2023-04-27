#include "render.h"

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
    const std::string vertex_shader_text = loadShader("lighting_vertex_shader");
    const std::string fragment_shader_text = loadShader("lighting_fragment_shader");
    const GLchar* source;
    GLint isCompiled;


    // Shaders
    source = (GLchar *) vertex_shader_text.c_str();
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


    source = (GLchar *) fragment_shader_text.c_str();
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


    shader = glCreateProgram();
    glAttachShader(shader, fs);
    glAttachShader(shader, vs);
    glLinkProgram(shader);

    GLint program_linked;
    glGetProgramiv(shader, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
      GLsizei log_length = 0;
      GLchar message[1024];
      glGetProgramInfoLog(shader, 1024, &log_length, message);
      for (auto c: message) {
        std::cout << c;
      }


    }

    texture = 0;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("../resources/textures/uv_grid_opengl.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        if (stbi_failure_reason()) std::cout << stbi_failure_reason();
    }

    stbi_image_free(data);
}

void Renderer::render() {


  glUseProgram(shader);

  GLint uniformMVP = glGetUniformLocation(shader, "MVP");
  GLint uniformCameraVector = glGetUniformLocation(shader, "cameraVector");

  glm::mat4 VP = this->perspectiveMatrix * camera.getCameraMatrix();

  glUniform3fv(uniformCameraVector, 1, &camera.lookatDirection[0]);


  for (auto& chunkPair : world.chunks)
  {
    glBindVertexArray(chunkPair.second->vao);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::mat4 translationMatrix = glm::translate(chunkPair.second->chunkPosition * (int) Chunk::CHUNK_SIZE);

    glm::mat4 MVP = VP * translationMatrix;

    glUniformMatrix4fv(uniformMVP, 1, false, &MVP[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, chunkPair.second->vertexPositions.size());   
  }
}
