#include "render.h"

static constexpr const char* vertex_shader_text = "#version 330\n"
"uniform mat4 MVP;\n"
"layout (location = 2) in vec2 vTexCoord;\n"
"layout (location = 1) in vec3 vCol;\n"
"layout (location = 0) in vec3 vPos;\n"
"out vec3 Color;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    Color = vCol;\n"
"    TexCoord = vTexCoord;\n"
"}\n";

static constexpr const char* fragment_shader_text = "#version 330\n"
"out vec4 FragColor;\n"
"in vec3 Color;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D uTexture;\n"
"void main()\n"
"{\n"
"    FragColor = texture(uTexture, TexCoord)\n"
"}\n";

Renderer::Renderer(GLFWwindow& window, Camera& camera, World& world) : window(window), camera(camera), world(world)   {
  perspectiveMatrix = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.f);
}

void Renderer::createGeometry() {
    // objects.push_back(
    //     Triangle{
    //         std::array<GLfloat, 3>{1.0, 0.0, 0.0},
    //         std::array<GLfloat, 3>{0.5, 0.0, 1.0},
    //         std::array<GLfloat, 3>{0.0, 0.0, 1.0},
    //     }
    // );

    points = { 
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(1.0f, 0.0f,  0.0f),
      glm::vec3(1.0f,  1.0f,  0.0f),
      glm::vec3(0.0f, 1.0f,  0.0f),
      glm::vec3(0.0f, 1.0f,  1.0f),
      glm::vec3(1.0f,  1.0f,  1.0f),
      glm::vec3(1.0f, 0.0f,  1.0f),
      glm::vec3(0.0f,  0.0f,  1.0f)
    };

    colors = { 
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(1.0f, 0.0f,  0.0f),
      glm::vec3(1.0f,  1.0f,  0.0f),
      glm::vec3(0.0f, 1.0f,  0.0f),
      glm::vec3(0.0f, 1.0f,  1.0f),
      glm::vec3(1.0f,  1.0f,  1.0f),
      glm::vec3(1.0f, 0.0f,  1.0f),
      glm::vec3(0.0f,  0.0f,  1.0f)
    };

    textureCoordinates = { 
      glm::vec2(0.0f,  0.0f),
      glm::vec2(1.0f,  0.0f),
      glm::vec2(1.0f,  1.0f),
      glm::vec2(0.0f,  1.0f),
      glm::vec2(0.0f,  0.0f),
      glm::vec2(1.0f,  0.0f),
      glm::vec2(1.0f,  1.0f),
      glm::vec2(0.0f,  1.0f)
    };

    indices = {
      0, 2, 1, //face front
      0, 3, 2,
      2, 3, 4, //face top
      2, 4, 5,
      1, 2, 5, //face right
      1, 5, 6,
      0, 7, 4, //face left
      0, 4, 3,
      5, 4, 7, //face back
      5, 7, 6,
      0, 6, 7, //face bottom
      0, 1, 6
    };
}


void Renderer::init() {
    createGeometry();


    // Shaders
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader_text, NULL);
    glCompileShader(vs);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader_text, NULL);
    glCompileShader(fs);

    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    // init vertex array object
    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // buffer for vertex data
    vertexBuffer = 0;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), &points[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // buffer for color data
    colorBuffer = 0;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), &colors[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    textureCoordinatesBuffer = 0;
    glGenBuffers(1, &textureCoordinatesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordinatesBuffer);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), &textureCoordinates[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    texture = 0;

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

    stbi_image_free(data);


    // buffer for index data
    indexBuffer = 0;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), &indices, GL_STATIC_DRAW);

}

void Renderer::render() {
  // wipe the drawing surface clear

  glUseProgram(shader_programme);
  glBindVertexArray(vao);

  GLint uniformMVP = glGetUniformLocation(shader_programme, "MVP");
  // glm::mat4 modelMatrix = glm::mat4(
  //   glm::vec4(1.0, 0.0, 0.0, 0.0),
  //   glm::vec4(0.0, 1.0, 0.0, 0.0),
  //   glm::vec4(0.0, 0.0, 1.0, 0.0),
  //   glm::vec4(0.0, 0.0, 0.0, 1.0)
  //   );

  glm::mat4 VP = this->perspectiveMatrix * camera.getCameraMatrix();

  for (std::shared_ptr<Block> block_ptr : world.visibleBlocks) {
    // Block block = pair.second;
    glm::mat4 MVP = VP * glm::translate(block_ptr->position);
    glUniformMatrix4fv(uniformMVP, 1, false, &MVP[0][0]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 8, GL_UNSIGNED_INT, 0);
  }

  /* for (int i = 0; i < world.sizeX; i++) {
    for (int j = 0; j < world.sizeY; j++) {
      for (int k = 0; k < world.sizeZ; k++) {
        Block &currentBlock = world.blocks[i][j][k];
        if (currentBlock.hidden || !currentBlock.isSolid() ) {
          continue;
        }
        glm::vec3 coordinates = currentBlock.position;
        // Block block = pair.second;
        glm::mat4 MVP = VP * glm::translate(coordinates);
        glUniformMatrix4fv(uniformMVP, 1, false, &MVP[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
      }
    }
  } */
}