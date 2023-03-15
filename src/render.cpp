#include "render.h"

Renderer::Renderer(GLFWwindow& window, Camera& camera, World& world) : window(window), camera(camera), world(world)   {
  perspectiveMatrix = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.f);
}

std::string Renderer::loadShader(const std::string& name) {
  std::ifstream ifs("../resources/shaders/" + name + ".glsl");
  std::string text((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()) );
  return text;
}


void Renderer::init() {
    // createGeometry();

    world.createMesh();

    const std::string vertex_shader_text = Renderer::loadShader("vertex_shader");
    const std::string fragment_shader_text = Renderer::loadShader("fragment_shader");
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


    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    GLint program_linked;
    glGetProgramiv(shader_programme, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
      GLsizei log_length = 0;
      GLchar message[1024];
      glGetProgramInfoLog(shader_programme, 1024, &log_length, message);
      for (auto c: message) {
        std::cout << c;
      }


    }



    // init vertex array object
    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // buffer for vertex data
    vertexBuffer = 0;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * world.vertexPositions.size() * sizeof(GLfloat), &world.vertexPositions[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // buffer for color data
    colorBuffer = 0;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * world.vertexColors.size() * sizeof(GLfloat), &world.vertexColors[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

/*
    textureCoordinatesBuffer = 0;
    glGenBuffers(1, &textureCoordinatesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordinatesBuffer);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), &textureCoordinates[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL); */

    /* texture = 0;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("../resources/arrow.jpg", &width, &height, &nrChannels, 0);
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

    stbi_image_free(data); */

    std::vector<GLuint> indices = std::vector<GLuint>();
    for (GLuint i = 0; i < world.vertexPositions.size(); ++i) {
      indices.push_back(i);
    }

    // buffer for index data
    indexBuffer = 0;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

void Renderer::render() {

  glUseProgram(shader_programme);
  glBindVertexArray(vao);

  GLint uniformMVP = glGetUniformLocation(shader_programme, "MVP");

  glm::mat4 VP = this->perspectiveMatrix * camera.getCameraMatrix();
  glm::mat4 MVP = VP;

  glUniformMatrix4fv(uniformMVP, 1, false, &MVP[0][0]);
  glDrawElements(GL_TRIANGLES, world.vertexPositions.size(), GL_UNSIGNED_INT, 0);    
}
