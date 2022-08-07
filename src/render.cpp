#include "render.h"


Renderer::Renderer(GLFWwindow& window, Camera& camera) : window(window), camera(camera)   {}

void Renderer::createGeometry() {
    // objects.push_back(
    //     Triangle{
    //         std::array<GLfloat, 3>{1.0, 0.0, 0.0},
    //         std::array<GLfloat, 3>{0.5, 0.0, 1.0},
    //         std::array<GLfloat, 3>{0.0, 0.0, 1.0},
    //     }
    // );

    points = { 
        glm::vec3(0.0f,  0.5f,  0.0f),
        glm::vec3(0.5f, -0.5f,  0.0f),
        glm::vec3(-0.5f, -0.5f,  0.0f)
    };

    colors = { 
        glm::vec3(1.0f,  0.5f,  0.0f),
        glm::vec3(0.5f, -0.5f,  0.0f),
        glm::vec3(-0.5f, -0.5f,  0.0f)
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

    vertexBuffer = 0;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), &points[0][0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


    colorBuffer = 0;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), &colors[0][0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}

void Renderer::render() {
  // wipe the drawing surface clear

  glUseProgram(shader_programme);
  glBindVertexArray(vao);

  // 
  GLint uniformMVP = glGetUniformLocation(shader_programme, "MVP");
  glm::mat4 modelMatrix = glm::mat4(
    glm::vec4(1.0, 0.0, 0.0, 0.0),
    glm::vec4(0.0, 1.0, 0.0, 0.0),
    glm::vec4(0.0, 0.0, 1.0, 0.0),
    glm::vec4(0.0, 0.0, 0.0, 1.0)
    );
  glm::mat4 perspectiveMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.f);
  glm::mat4 MVP = perspectiveMatrix * camera.getCameraMatrix() * modelMatrix;
  glUniformMatrix4fv(uniformMVP, 1, false, &MVP[0][0]);
  // draw points 0-3 from the currently bound VAO with current in-use shader
  glDrawArrays(GL_TRIANGLES, 0, 3);

}